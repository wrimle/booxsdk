// Author: John

#ifndef CMS_DOWNLOAD_DB_H_
#define CMS_DOWNLOAD_DB_H_

#include <QtCore/QtCore>
#include <QtSql/QtSql>
#include "onyx/base/base.h"

namespace cms
{

enum DownloadState
{
    STATE_INVALID = -1,
    DOWNLOADING,
    FINISHED,
    FAILED,
};

class DownloadItemInfo : public  QVariantMap
{
public:
    explicit DownloadItemInfo(const QVariantMap & vm = QVariantMap());
    ~DownloadItemInfo();

    bool operator == (const DownloadItemInfo &right);

    QString url() const;
    void setUrl(const QString & url);

    QString path() const;
    void setPath(const QString & path);

    int size() const;
    void setSize(int size);

    DownloadState state() const;
    void setState(DownloadState state);

    QString timeStamp() const;
    void setTimeStamp(const QString & timeStamp);
};

typedef QVector<DownloadItemInfo> DownloadInfoList;

struct LessByUrl
{
    bool operator()( const DownloadItemInfo & a, const DownloadItemInfo & b ) const
    {
        return (a.url().compare(b.url(), Qt::CaseInsensitive) < 0);
    }
};

struct GreaterByUrl
{
    bool operator()( const DownloadItemInfo & a, const DownloadItemInfo & b ) const
    {
        return (a.url().compare(b.url(), Qt::CaseInsensitive) > 0);
    }
};

struct LessByTimestamp
{
    bool operator()( const DownloadItemInfo & a, const DownloadItemInfo & b ) const
    {
        return (a.timeStamp().compare(b.timeStamp(), Qt::CaseInsensitive) < 0);
    }
};

struct GreaterByTimestamp
{
    bool operator()( const DownloadItemInfo & a, const DownloadItemInfo & b ) const
    {
        return (a.timeStamp().compare(b.timeStamp(), Qt::CaseInsensitive) > 0);
    }
};

class DownloadDB
{
public:
    DownloadDB(const QString & db_name = "download.db");
    ~DownloadDB();

public:
    bool open();
    bool close();

    DownloadInfoList list();
    DownloadInfoList pendingList(const DownloadInfoList & input = DownloadInfoList(),
                             bool force_all = false,
                             bool sort = true);

    bool update(const DownloadItemInfo & item);
    bool updateState(const QString & url, DownloadState state = FINISHED);

private:
    bool makeSureTableExist(QSqlDatabase &db);
    QSqlDatabase & db();

private:
    scoped_ptr<QSqlDatabase> database_;

    const QString database_name_;

};


}  // namespace cms

#endif  // CMS_DOWNLOAD_DB_H_
