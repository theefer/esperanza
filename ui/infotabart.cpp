#include "infotabart.h"

#include <QGridLayout>
#include <QHash>
#include <QVariant>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QProgressDialog>
#include <QBuffer>
#include <QByteArray>
#include <QTimer>

InfoTabArt::InfoTabArt (QWidget *parent, XClient *client) :
	InfoWindowTab (parent, client)
{
	QGridLayout *g = new QGridLayout (this);
	g->setMargin (1);
	setLayout (g);

	m_art = new QLabel (this);
	m_art->setAlignment (Qt::AlignCenter);
	m_art->setText (tr ("No Image"));
	g->addWidget (m_art, 0, 0);
	g->setRowStretch (0, 1);

	QFrame *line = new QFrame (this);
	line->setFrameStyle (QFrame::HLine | QFrame::Raised);
	g->addWidget (line, 1, 0);

	QDialogButtonBox *bbox = new QDialogButtonBox (this);

	m_browse = new QPushButton (tr ("Select"));
	m_browse->setDefault (true);
	connect (m_browse, SIGNAL (clicked ()), this, SLOT (browse ()));

	m_apply = new QPushButton (tr ("Apply"));
	connect (m_apply, SIGNAL (clicked ()), this, SLOT (apply ()));
	m_apply->setEnabled (false);

	m_remove = new QPushButton (tr ("Remove"));
	connect (m_remove, SIGNAL (clicked ()), this, SLOT (remove ()));
	m_remove->setEnabled (false);

	bbox->addButton (m_browse, QDialogButtonBox::ActionRole);
	bbox->addButton (m_apply, QDialogButtonBox::ApplyRole);
	bbox->addButton (m_remove, QDialogButtonBox::DestructiveRole);
	g->addWidget (bbox, 2, 0);
}

void
InfoTabArt::remove ()
{
	int ret = QMessageBox::question (this, tr ("Esperanza - Question"),
									 tr ("Do you want to remove art for the whole album?\nPress yes to apply to the whole album and no to apply only to this song."),
									 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
									 QMessageBox::Yes);

	m_progress = new QProgressDialog (tr ("Remove art..."), tr ("Abort"), 0, 3, this);
	m_progress->setWindowModality (Qt::ApplicationModal);

	if (ret == QMessageBox::Yes) {
		remove_art (true);
	} else if (ret == QMessageBox::No) {
		remove_art (false);
	} else if (ret == QMessageBox::Cancel) {
	}
}

void
InfoTabArt::remove_art (bool many)
{
	QHash<QString, QVariant> minfo = m_client->cache ()->get_info (m_current_id);
	m_client->bindata.remove (minfo["picture_front"].toString ().toStdString (), &XClient::log);
	if (many) {
		m_progress->setValue (1);
		m_progress->setLabelText (tr ("Querying db..."));
		m_client->medialib.select (build_album_query (minfo).toStdString (),
								   Xmms::bind (&InfoTabArt::medialib_reply_remove, this));
	} else {
		m_client->medialib.entryPropertyRemove (m_current_id, "picture_front", &XClient::log);
		m_progress->setValue (3);
		m_progress->setLabelText (tr ("Finishing off..."));
		QTimer::singleShot (1200, this, SLOT (art_finished ()));
	}
}

bool
InfoTabArt::medialib_reply_remove (const Xmms::List< Xmms::Dict > &list)
{
	for (list.first (); list.isValid (); ++ list) {
		int32_t id = (*list).get<int32_t> ("id");
		m_client->medialib.entryPropertyRemove (id, "picture_front", &XClient::log);
	}

	m_progress->setValue (3);
	m_progress->setLabelText (tr ("Finishing off..."));
	QTimer::singleShot (1200, this, SLOT (art_finished ()));

	return true;
}

void
InfoTabArt::browse ()
{
	QString file = QFileDialog::getOpenFileName (this,
												 tr ("Select albumart"),
												 QDir::homePath (),
												 tr ("Images (*.png *.jpg)"));
	if (!file.isNull ()) {
		QPixmap p (file);
		setPixmap (p);
		m_browse->setDefault (false);
		m_apply->setEnabled (true);
		m_apply->setDefault (true);
		m_remove->setEnabled (false);
	}

}

void
InfoTabArt::apply ()
{
	int ret = QMessageBox::question (this, tr ("Esperanza - Question"),
									 tr ("Do you want to change art for the whole album?\nPress yes to apply to the whole album and no to apply only to this song."),
									 QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
									 QMessageBox::Yes);
	
	m_progress = new QProgressDialog (tr ("Adding art..."), tr ("Abort"), 0, 3, this);
	m_progress->setWindowModality (Qt::ApplicationModal);

	if (ret == QMessageBox::Yes) {
		add_art (true);
	} else if (ret == QMessageBox::No) {
		add_art (false);
	} else if (ret == QMessageBox::Cancel) {
	}
}

QString
InfoTabArt::build_album_query (const QHash<QString, QVariant> &minfo)
{
	QString q;

	if (minfo.contains ("album_id")) {
		q = "select id from Media where ";
		q.append (QString ("key='album_id' and value='%1'").arg (minfo["album_id"].toString ()));
	} else {
		q = "select m1.id from Media m1 join Media m2 on m1.id = m2.id where";
		q.append (QString ("m1.key='artist' and m1.value='%1' and m2.key='album' and m2.value='%2'").arg(minfo["artist"].toString ()).arg(minfo["album"].toString ()));
	}

	qDebug ("%s", qPrintable (q));

	return q;
}

bool
InfoTabArt::art_uploaded (const std::string &hash, bool many)
{
	QHash<QString, QVariant> minfo = m_client->cache ()->get_info (m_current_id);

	if (many) {
		m_progress->setValue (1);
		m_progress->setLabelText (tr ("Querying db..."));
		m_client->medialib.select (build_album_query (minfo).toStdString (),
								   boost::bind (&InfoTabArt::medialib_reply, this, _1, hash));
	} else {
		m_client->medialib.entryPropertySet (m_current_id, "picture_front", hash, &XClient::log);
		m_progress->setValue (3);
		m_progress->setLabelText (tr ("Finishing off..."));
		QTimer::singleShot (1200, this, SLOT (art_finished ()));
	}

	return true;
}

void
InfoTabArt::add_art (bool many)
{
	QByteArray ba;
	QBuffer buffer (&ba);
	buffer.open (QIODevice::WriteOnly);
	m_art->pixmap ()->save (&buffer, "JPG");

	m_client->bindata.add (Xmms::bin ((unsigned char *)ba.data (), ba.size ()),
						   boost::bind (&InfoTabArt::art_uploaded, this, _1, many));
}

bool
InfoTabArt::medialib_reply (const Xmms::List< Xmms::Dict > &list,
							const std::string &hash)
{
	qDebug ("paaaa!");
	m_progress->setValue (2);
	m_progress->setLabelText (tr ("Adding art to entries..."));

	for (list.first (); list.isValid (); ++ list) {
		int32_t id = (*list).get<int32_t> ("id");
		m_client->medialib.entryPropertySet (id, "picture_front", hash, &XClient::log);
	}

	m_progress->setValue (3);
	m_progress->setLabelText (tr ("Finishing off..."));
	QTimer::singleShot (1200, this, SLOT (art_finished ()));

	return true;
}

void
InfoTabArt::art_finished ()
{
	m_progress->close ();
	m_apply->setEnabled (false);
	m_browse->setDefault (true);
	m_remove->setEnabled (true);
}

void
InfoTabArt::fill (uint32_t id)
{
	m_current_id = id;
	QPixmap p = m_client->cache ()->get_pixmap (id);
	setPixmap (p);
	if (!p.isNull ())
		m_remove->setEnabled (true);
}

void
InfoTabArt::setPixmap (const QPixmap &p)
{
	if (p.isNull ()) {
		m_art->setPixmap (QPixmap ());
		m_art->setText (tr ("No Image"));
	} else {
		if (p.width () > 220) {
			QPixmap small = p.scaledToWidth (220, Qt::SmoothTransformation);
			m_art->setPixmap (small);
		} else {
			m_art->setPixmap (p);
		}
	}
}

