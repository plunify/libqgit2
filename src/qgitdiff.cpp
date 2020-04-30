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

#include "qgitdiff.h"
#include "qgitdiffdelta.h"
#include "qgitdiffstats.h"

namespace LibQGit2
{

Diff::Diff(git_diff *diff) :
    d(diff, git_diff_free)
{
}

size_t Diff::numDeltas() const
{
    size_t ret = 0;
    if (!d.isNull()) {
        ret = git_diff_num_deltas(d.data());
    }
    return ret;
}

DiffDelta Diff::delta(size_t index) const
{
    const git_diff_delta *delta = 0;
    if (!d.isNull()) {
        delta = git_diff_get_delta(d.data(), index);
    }
    return DiffDelta(delta);
}

DiffStats Diff::stats() const
{
    int ret = 0;
    git_diff_stats *stats = 0;
    if (!d.isNull()) {
         ret = git_diff_get_stats(&stats, d.data());
    }
    // TODO: handle error
    return DiffStats(stats);
}

QString Diff::patch(size_t index) const
{
    git_patch *patch = NULL;
    int error = git_patch_from_diff(&patch, d.data(), index);
    QString ret;
    if(!error){
        git_buf b = GIT_BUF_INIT_CONST(NULL, 0);
        git_patch_to_buf(&b, patch);
        ret = b.ptr;
        git_buf_free(&b);
        git_patch_free(patch);
    }
    return ret;
}

}
