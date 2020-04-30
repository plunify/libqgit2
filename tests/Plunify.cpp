#include "TestHelpers.h"

#include "qgitglobal.h"
#include "qgitrepository.h"
#include "qgittree.h"
#include "qgitdiff.h"
#include "qgitdiffdelta.h"
#include "qgitdifffile.h"
#include "qgitdiffstats.h"
#include "qgitstatuslist.h"
#include "qgitstatusentry.h"
#include "qgitstatus.h"
#include "qgitstatusoptions.h"
#include <QDir>
#include <QDebug>

using namespace LibQGit2;

class TestPlunify : public QObject
{
    Q_OBJECT

public:
    TestPlunify();
    ~TestPlunify();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void plunify();

private:
    void status(LibQGit2::Repository repo);
};

TestPlunify::TestPlunify()
{
}

void TestPlunify::initTestCase()
{
    initLibQGit2();
}

void TestPlunify::cleanupTestCase()
{
    shutdownLibQGit2();
}

void TestPlunify::status(LibQGit2::Repository repo)
{
    // git status
    StatusOptions opt;
    opt.setShowType(StatusOptions::ShowIndexAndWorkdir);
    opt.setStatusFlags(StatusOptions::IncludeUnmodified | StatusOptions::IncludeUntracked |
        StatusOptions::RenamesHeadToIndex | StatusOptions::RenamesIndexToWorkdir
    );

    StatusList status_list = repo.status(opt);

    size_t entries = status_list.entryCount();
    for (size_t i = 0; i < entries; ++i) {
        const StatusEntry entry = status_list.entryByIndex(i);
        QString output;

        if (entry.status().isCurrent()) {
            output.append("C");
        } else {
            output.append(" ");
        }

        if (entry.status().isNewInIndex()) {
            output.append("N");
        } else {
            output.append(" ");
        }

        if (entry.status().isModifiedInIndex()) {
            output.append("M");
        } else {
            output.append(" ");
        }

        if (entry.status().isDeletedInIndex()) {
            output.append("D");
        } else {
            output.append(" ");
        }

        if (entry.status().isRenamedInIndex()) {
            output.append("R");
        } else {
            output.append(" ");
        }

        if (entry.status().isTypeChangedInIndex()) {
            output.append("T");
        } else {
            output.append(" ");
        }

        output.append(" ");

        if (entry.status().isNewInWorkdir()) {
            output.append("N");
        } else {
            output.append(" ");
        }

        if (entry.status().isModifiedInWorkdir()) {
            output.append("M");
        } else {
            output.append(" ");
        }

        if (entry.status().isDeletedInWorkdir()) {
            output.append("D");
        } else {
            output.append(" ");
        }

        if (entry.status().isRenamedInWorkdir()) {
            output.append("R");
        } else {
            output.append(" ");
        }

        if (entry.status().isTypeChangedInWorkdir()) {
            output.append("T");
        } else {
            output.append(" ");
        }

        output.append(" ");
        output.append(entry.indexToWorkdir().newFile().path());
        qDebug() << output;
    }

}

void TestPlunify::plunify()
{
    LibQGit2::Repository repo;
    const QString repoPath("c:/projects/test/plunify.git");

    qDebug() << "git init";
    try {
        repo.init(repoPath, false);
    } catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    qDebug() << "git status";
    status(repo);

    qDebug() << "\n\ngit add";
    LibQGit2::Index index;
    try {
        index = repo.index();
        index.addAll();
        index.updateAll();
    } catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

//    qDebug() << "\n\ngit status";
//    status(repo);

    qDebug() << "\n\ngit diff";
    Tree oldTree = repo.lookupRevision("HEAD^{tree}").toTree();
    Diff diff = repo.diffTreeToWorkDirWithIndex(oldTree);
    size_t numD = diff.numDeltas();
    qDebug() << "numDelta = " << numD;
    QStringList deltaTypes;

    deltaTypes << "Unmodified" << "Added" << "Deleted" << "Modified" << "Renamed" << "Copied"
               << "Ignored" << "Untracked" << "Typechange";
    for(int i = 0; i < numD; ++i){
        DiffDelta delta = diff.delta(i);
        qDebug() << deltaTypes[int(delta.type())] << " " << delta.oldFile().path();
                                                  //<< " " << delta.newFile().path();
    }

    qDebug() << "\n\ngit diff --stat";

    DiffStats stats = diff.stats();
    qDebug() << " Files Changed: " << stats.filesChanged()
            << " Insertions: " << stats.insertions() << " Deletions: " << stats.deletions();

    QString fmt_stat = "Stats:\n";
    fmt_stat.append(stats.format(DiffStats::Full, 80));
    qDebug() << fmt_stat;

}



TestPlunify::~TestPlunify()
{
}

QTEST_MAIN(TestPlunify);

#include "Plunify.moc"
