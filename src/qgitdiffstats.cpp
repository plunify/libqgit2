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

#include "qgitdiffstats.h"
#include "qgitdifffile.h"

#include <QString>

namespace LibQGit2 {

DiffStats::DiffStats(git_diff_stats *d) : m_diff_stats(d)
{
}

DiffStats::~DiffStats()
{
    git_diff_stats_free(m_diff_stats);
}

size_t DiffStats::insertions()
{
    return git_diff_stats_insertions(m_diff_stats);
}

size_t DiffStats::deletions()
{
    return git_diff_stats_deletions(m_diff_stats);
}

size_t DiffStats::filesChanged()
{
    return git_diff_stats_files_changed(m_diff_stats);
}

QString DiffStats::format(Format format, int width)
{
    git_buf b = GIT_BUF_INIT_CONST(NULL, 0);
    git_diff_stats_to_buf(&b, m_diff_stats, git_diff_stats_format_t(format), width);
    QString ret(b.ptr);
    git_buf_free(&b);
    return ret;
}

}
