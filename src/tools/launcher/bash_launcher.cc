// Copyright 2017 The Bazel Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <sstream>
#include <string>
#include <vector>

#include "src/tools/launcher/bash_launcher.h"
#include "src/tools/launcher/util/launcher_util.h"

namespace bazel {
namespace launcher {

using std::vector;
using std::wostringstream;
using std::wstring;

static constexpr const char* BASH_BIN_PATH = "bash_bin_path";

ExitCode BashBinaryLauncher::Launch() {
  wstring bash_binary = this->GetLaunchInfoByKey(BASH_BIN_PATH);
  // If specified bash binary path doesn't exist, then fall back to
  // bash.exe and hope it's in PATH.
  if (!DoesFilePathExist(bash_binary.c_str())) {
    bash_binary = L"bash.exe";
  }

  vector<wstring> origin_args = this->GetCommandlineArguments();
  wostringstream bash_command;
  wstring bash_main_file = GetBinaryPathWithoutExtension(origin_args[0]);
  bash_command << GetEscapedArgument(bash_main_file,
                                     /*escape_backslash = */ true);
  for (int i = 1; i < origin_args.size(); i++) {
    bash_command << L' ';
    bash_command << GetEscapedArgument(origin_args[i],
                                       /*escape_backslash = */ true);
  }

  vector<wstring> args;
  args.push_back(L"-c");
  args.push_back(
      GetEscapedArgument(bash_command.str(), /*escape_backslash = */ true));
  return this->LaunchProcess(bash_binary, args);
}

}  // namespace launcher
}  // namespace bazel
