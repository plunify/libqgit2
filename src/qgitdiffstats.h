/******************************************************************************
 * This file is part of the libqgit2 library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBQGIT2_DIFFSTATS_H
#define LIBQGIT2_DIFFSTATS_H

#include "libqgit2_config.h"

#include "git2.h"

#include <QString>

namespace LibQGit2 {

class DiffFile;

/**
 * @brief Wrapper class for git_diff_stats.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT DiffStats
{
public:
    DiffStats(git_diff_stats *d);
    ~DiffStats();

    //TODO
    enum Format {

        None = GIT_DIFF_STATS_NONE,                      ///<  No stats
        Full = GIT_DIFF_STATS_FULL,                      ///<  Full statistics, equivalent of `--stat`
        Short = GIT_DIFF_STATS_SHORT,                    ///<  Short statistics, equivalent of `--shortstat`
        Number = GIT_DIFF_STATS_NUMBER,                  ///<  Number statistics, equivalent of `--numstat`
        IncludeSummary = GIT_DIFF_STATS_INCLUDE_SUMMARY, ///<  Extended header information such as creations, renames and mode changes, equivalent of `--summary`
    };

    /**
     * @brief Total number of insertions
     */
    size_t insertions();

    /**
     * @brief Total number of deletions
     */
    size_t deletions();

    /**
     * @brief Total number of files changed
     */
    size_t filesChanged();

    /**
     * @brief Format the stats as a string.
     * @param format The format to use. A \c Format enum.
     * @param width The width of the output. The output will be scaled to fit.
     * @return
     */
    QString format(Format format, int width);

private:
    git_diff_stats *m_diff_stats;
};

/** @} */

}

#endif // LIBQGIT2_DIFFSTATS_H
