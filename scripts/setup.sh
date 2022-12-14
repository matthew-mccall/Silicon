#!/usr/bin/env bash

#
# BSD 2-Clause License
#
# Copyright (c) 2022, Matthew McCall
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

if [ ! -f boost/version.hpp ]; then
  if [ ! -f bootstrap.sh ] ; then
    if ! pwd | grep -q "/libs/boost" ; then
      set HAS_CD
      if pwd | grep -q "/scripts" ; then
        cd ../libs/boost || exit
      else
        cd ./libs/boost || exit
      fi
    fi
  fi

  if [ ! -f boost/version.hpp ]; then
    echo "Initializing Boost for Silicon Desktop..."

    ./bootstrap.sh
    ./b2 headers
  else
    echo "Found Boost for Silicon Desktop!"
  fi
fi

if [ -n "$HAS_CD" ]; then
  cd - || exit
fi

unset HAS_CD