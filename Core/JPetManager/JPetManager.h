/**
 *  @copyright Copyright 2018 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetManager.h
 */

#ifndef JPETMANAGER_H
#define JPETMANAGER_H

#include "./JPetTaskChainExecutor/JPetTaskChainExecutor.h"
#include "./JPetOptionsTools/JPetOptionsTools.h"
#include <memory>
#include <map>

/**
 * @brief Main Manager of the analyses performed with the J-PET Framework.
 *
 * Each analysis program needs an instance of the JPetManager,
 * which is responsible for parsing the command line arguments,
 * registering processing tasks, and sending it to JPetExecutor,
 * which executes the chain of registered tasks in threads.
 * Private fields include Options container, in which the number of elements
 * corresponds to the number of independent input files
 */
class JPetManager
{
public:
  using Options = std::map<std::string, jpet_options_tools::OptsStrAny>;
  using TaskGenerator = std::function<JPetTaskInterface* ()>;
  using TaskGeneratorChain = std::vector<TaskGenerator>;
  static JPetManager& getManager();
  ~JPetManager();
  Options getOptions() const;
  bool run(int argc, const char** argv);
  bool parseCmdLine(int argc, const char** argv);
  bool areThreadsEnabled() const;
  void setThreadsEnabled(bool enable);
  void useTask(const char * name,
	       const char * inputFileType="",
	       const char * outputFileType="");
  void clearRegisteredTasks();

  /**
   * Template type method for registering tasks. After registering the task,
   * the user should also invoke Managers method 'useTask'.
   */
  template<typename T> void registerTask(const char * name){
    fTasksDictionary[name] = [name]() {
      return new T(name);
    };
  }

private:
  JPetManager();
  JPetManager(const JPetManager&);
  void operator=(const JPetManager&);
  void insertTaskIntoChain(const char * name,
	       const char * inputFileType="",
	       const char * outputFileType="",
	       bool firstTask=false);
  std::map<const char*, TaskGenerator> fTasksDictionary;
  TaskGeneratorChain* fTaskGeneratorChain = nullptr;
  bool fThreadsEnabled = false;
  Options fOptions;
};
#endif /* !JPETMANAGER_H */
