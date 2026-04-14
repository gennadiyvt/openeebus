/*
 * Copyright 2025 NIBE AB
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 * @brief Currently it is not a regular unit test but more a "sand box"
 * to feed the SPINE Device with specific datagrams and check the outgoing messages printed.
 * @note Remember to enable the message printing in PrintMessage() before getting started
 */

#include "src/use_case/actor/ma/mpc/ma_mpc.h"

#include <gtest/gtest.h>

#include <map>
#include <memory>

#include "mocks/common/eebus_timer/eebus_timer_mock.h"
#include "mocks/ship/ship_connection/data_writer_mock.h"
#include "mocks/use_case/api/ma_mpc_listener_mock.h"
#include "src/common/array_util.h"
#include "src/common/eebus_malloc.h"
#include "src/common/eebus_timer/eebus_timer.h"
#include "src/common/message_buffer.h"
#include "src/spine/device/device_local.h"
#include "src/spine/device/device_local_internal.h"
#include "src/spine/entity/entity_local.h"
#include "tests/src/json.h"
#include "tests/src/use_case/actor/ma/mpc/receive/discovery_request.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/discovery_response.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/electrical_connection_description_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/electrical_connection_parameter_description_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_constraints_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_description_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_notify_current.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_notify_energy.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_notify_frequency.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_notify_power.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_notify_voltage.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/measurement_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/node_management_subscription_request.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/result_data_msg_cnt_ref_3.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/result_data_msg_cnt_ref_5.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/result_data_msg_cnt_ref_8.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/use_case_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/receive/use_case_request.inc"
#include "tests/src/use_case/actor/ma/mpc/send/discovery_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/discovery_reply.inc"
#include "tests/src/use_case/actor/ma/mpc/send/electrical_connection_description_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/electrical_connection_parameter_description_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/electrical_connection_subscription_call.inc"
#include "tests/src/use_case/actor/ma/mpc/send/measurement_constraints_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/measurement_description_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/measurement_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/measurement_subscription_call.inc"
#include "tests/src/use_case/actor/ma/mpc/send/node_management_subscription_call.inc"
#include "tests/src/use_case/actor/ma/mpc/send/result_data_msg_cnt_ref_3.inc"
#include "tests/src/use_case/actor/ma/mpc/send/use_case_data_read.inc"
#include "tests/src/use_case/actor/ma/mpc/send/use_case_data_reply.inc"
#include "tests/src/use_case/use_case_test_fixture.h"

using testing::_;
using testing::Return;

namespace ma_mpc_test {

class MaMpcTestFixture : public UseCaseTestFixture {
 public:
  MaMpcTestFixture() : UseCaseTestFixture("HEMS", "HEMS", "123456789") {};
  void SetUpUseCase() override {
    uint32_t entity_ids[1]{static_cast<uint32_t>(VectorGetSize(DEVICE_LOCAL_GET_ENTITIES(device_local_.get())))};

    EntityLocalObject* const entity = EntityLocalCreate(
        device_local_.get(),
        kEntityTypeTypeCEM,
        entity_ids,
        ARRAY_SIZE(entity_ids),
        kHeartbeatTimeout
    );

    ma_mpc_listener_mock_.reset(MaMpcListenerMockCreate());
    use_case_.reset(MaMpcUseCaseCreate(entity, MA_MPC_LISTENER_OBJECT(ma_mpc_listener_mock_.get())));

    DEVICE_LOCAL_ADD_ENTITY(device_local_.get(), entity);
    ExpectSendMessage(send::discovery_read);
  };

  void TearDownUseCase() override {
    EXPECT_CALL(*ma_mpc_listener_mock_->gmock, Destruct(_)).WillOnce(Return());
    use_case_.reset();
    ma_mpc_listener_mock_.reset();
  };

  void ExpectMeasurementsReceive(
      MaMpcListenerGMock* mock,
      const std::map<MuMpcMeasurementNameId, ScaledValue>& expected_measurements
  ) {
    for (const auto& [name_id, scaled_value] : expected_measurements) {
      const int64_t value{scaled_value.value};
      const int8_t scale{scaled_value.scale};
      EXPECT_CALL(*mock, OnMeasurementReceive(_, name_id, ScaledValueEq(value, scale), _)).WillOnce(Return());
    }
  }

 protected:
  std::unique_ptr<MaMpcListenerMock, decltype(&MaMpcListenerMockDelete)> ma_mpc_listener_mock_{
      nullptr,
      MaMpcListenerMockDelete
  };

  std::unique_ptr<MaMpcUseCaseObject, decltype(&MaMpcUseCaseDelete)> use_case_{nullptr, MaMpcUseCaseDelete};
};

TEST_F(MaMpcTestFixture, MaMpcTest) {
  // 1. Receive the detailed discovery request and send the response
  ExpectSendMessage(send::discovery_reply);
  HandleMessage(receive::discovery_request);

  // 2. Receive the detailed discovery response and send subscriptions + use case read
  EXPECT_CALL(*ma_mpc_listener_mock_->gmock, OnRemoteEntityConnect(_, _)).WillOnce(Return());
  ExpectSendMessage(send::node_management_subscription_call);
  ExpectSendMessage(send::use_case_data_read);
  HandleMessage(receive::discovery_response);

  // 3. Receive the Node Management subscription request and send result
  ExpectSendMessage(send::result_data_msg_cnt_ref_3);
  HandleMessage(receive::node_management_subscription_request);

  // 4. Receive the use case discovery request and send the reply
  ExpectSendMessage(send::use_case_data_reply);
  HandleMessage(receive::use_case_request);

  // 5. Receive the result with message counter reference 3
  HandleMessage(receive::result_data_msg_cnt_ref_3);

  // 6. Receive the Use Case reply and send electrical connection + measurement subscriptions and reads
  ExpectSendMessage(send::electrical_connection_subscription_call);
  ExpectSendMessage(send::electrical_connection_description_read);
  ExpectSendMessage(send::electrical_connection_parameter_description_read);
  ExpectSendMessage(send::measurement_subscription_call);
  ExpectSendMessage(send::measurement_description_read);
  ExpectSendMessage(send::measurement_constraints_read);
  HandleMessage(receive::use_case_reply);

  // 7. Receive the result with message counter reference 5
  HandleMessage(receive::result_data_msg_cnt_ref_5);

  // 8. Receive the electrical connection description reply
  HandleMessage(receive::electrical_connection_description_reply);

  // 9. Receive the electrical connection parameter description reply
  HandleMessage(receive::electrical_connection_parameter_description_reply);

  // 10. Receive the result with message counter reference 8
  HandleMessage(receive::result_data_msg_cnt_ref_8);

  // 11. Receive the measurement description reply and send the measurement read
  ExpectSendMessage(send::measurement_read);
  HandleMessage(receive::measurement_description_reply);

  // 12. Receive the measurement constraints reply
  HandleMessage(receive::measurement_constraints_reply);

  // 13. Receive the measurement reply
  EXPECT_CALL(*ma_mpc_listener_mock_->gmock, OnMeasurementReceive(_, kMpcPowerTotal, ScaledValueEq(33000, -1), _))
      .WillOnce(Return());

  HandleMessage(receive::measurement_reply);

  // 14. Receive the measurement notify (power)
  const std::map<MuMpcMeasurementNameId, ScaledValue> expected_power{
      {kMpcPowerPhaseA, {.value = 1000, .scale = 0}},
      {kMpcPowerPhaseB, {.value = 1100, .scale = 0}},
      {kMpcPowerPhaseC, {.value = 1200, .scale = 0}},
  };

  ExpectMeasurementsReceive(ma_mpc_listener_mock_->gmock, expected_power);
  HandleMessage(receive::measurement_notify_power);

  // 15. Receive the measurement notify (energy)
  const std::map<MuMpcMeasurementNameId, ScaledValue> expected_energy{
      {kMpcEnergyConsumed,  {.value = 550000, .scale = 0}},
      {kMpcEnergyProduced, {.value = 210007, .scale = -1}},
  };

  ExpectMeasurementsReceive(ma_mpc_listener_mock_->gmock, expected_energy);
  HandleMessage(receive::measurement_notify_energy);

  // 16. Receive the measurement notify (current)
  const std::map<MuMpcMeasurementNameId, ScaledValue> expected_current{
      {kMpcCurrentPhaseA, {.value = 33, .scale = -2}},
      {kMpcCurrentPhaseB, {.value = 51, .scale = -2}},
      {kMpcCurrentPhaseC, {.value = 13, .scale = -3}},
  };

  ExpectMeasurementsReceive(ma_mpc_listener_mock_->gmock, expected_current);
  HandleMessage(receive::measurement_notify_current);

  // 17. Receive the measurement notify (voltage)
  const std::map<MuMpcMeasurementNameId, ScaledValue> expected_voltage{
      { kMpcVoltagePhaseA,    {.value = 110, .scale = 0}},
      { kMpcVoltagePhaseB,  {.value = 1205, .scale = -1}},
      { kMpcVoltagePhaseC,    {.value = 130, .scale = 0}},
      {kMpcVoltagePhaseAb, {.value = 14037, .scale = -2}},
      {kMpcVoltagePhaseBc,    {.value = 150, .scale = 0}},
      {kMpcVoltagePhaseAc,     {.value = 16, .scale = 1}},
  };

  ExpectMeasurementsReceive(ma_mpc_listener_mock_->gmock, expected_voltage);
  HandleMessage(receive::measurement_notify_voltage);

  // 18. Receive the measurement notify (frequency)
  const std::map<MuMpcMeasurementNameId, ScaledValue> expected_frequency{
      {kMpcFrequency, {.value = 500, .scale = -1}},
  };

  ExpectMeasurementsReceive(ma_mpc_listener_mock_->gmock, expected_frequency);
  HandleMessage(receive::measurement_notify_frequency);

  // 19. Get all of the measurements received via GetMeasurementData() and check the values
  ScaledValue value;
  static constexpr uint32_t remote_entity_id = 1;

  static constexpr const uint32_t* const remote_entity_ids[] = {&remote_entity_id};

  const EntityAddressType remote_entity_addr
      = {"d:_n:HeatPump_123456789", remote_entity_ids, ARRAY_SIZE(remote_entity_ids)};

  std::map<MuMpcMeasurementNameId, ScaledValue> expected_data = expected_power;
  expected_data.insert(expected_energy.begin(), expected_energy.end());
  expected_data.insert(expected_current.begin(), expected_current.end());
  expected_data.insert(expected_voltage.begin(), expected_voltage.end());
  expected_data.insert(expected_frequency.begin(), expected_frequency.end());

  for (const auto& [name_id, scaled_value] : expected_data) {
    EXPECT_EQ(MaMpcGetMeasurementData(use_case_.get(), name_id, &remote_entity_addr, &value), kEebusErrorOk);
    EXPECT_THAT(&value, ScaledValueEq(scaled_value.value, scaled_value.scale));
  }

  // 20. Expect the remote entity disconnect event while tearing down the use case
  EXPECT_CALL(*ma_mpc_listener_mock_->gmock, OnRemoteEntityDisconnect(_, _));
}

}  // namespace ma_mpc_test
