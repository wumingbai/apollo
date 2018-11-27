/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <gtest/gtest.h>

#include "cyber/cyber.h"
#include "cyber/scheduler/policy/choreography.h"
#include "cyber/scheduler/policy/classic.h"
#include "cyber/scheduler/processor.h"
#include "cyber/scheduler/policy/scheduler_classic.h"
#include "cyber/scheduler/policy/scheduler_choreography.h"
#include "cyber/common/global_data.h"

namespace apollo {
namespace cyber {
namespace scheduler {

void func() {}
TEST(SchedulerPolicyTest, choreo) {
  auto processor = std::make_shared<Processor>();
  auto ctx = std::make_shared<ChoreographyContext>();
  processor->BindContext(ctx);
  ctx->BindProc(processor);

  std::shared_ptr<CRoutine> cr = std::make_shared<CRoutine>(func);
  auto task_id = GlobalData::RegisterTaskName("choreo");
  cr->set_id(task_id);
  EXPECT_TRUE(static_cast<ChoreographyContext*>(ctx.get())->Enqueue(cr));
  ctx->ShutDown();
}

TEST(SchedulerPolicyTest, classic) {
  auto processor = std::make_shared<Processor>();
  auto ctx = std::make_shared<ClassicContext>();
  processor->BindContext(ctx);
  ctx->BindProc(processor);
  ctx->ShutDown();
}

TEST(SchedulerPolicyTest, sched_classic) {
  GlobalData::Instance()->SetProcessGroup("example_classic_sched");
  auto sched1 = std::make_shared<SchedulerClassic>();
  std::shared_ptr<CRoutine> cr = std::make_shared<CRoutine>(func);
  auto task_id = GlobalData::RegisterTaskName("ABC");
  cr->set_id(task_id);
  EXPECT_TRUE(sched1->DispatchTask(cr));
  // dispatch the same task
  EXPECT_FALSE(sched1->DispatchTask(cr));
  EXPECT_TRUE(sched1->RemoveTask("ABC"));
  sched1->ShutDown();
}

}  // namespace scheduler
}  // namespace cyber
}  // namespace apollo
