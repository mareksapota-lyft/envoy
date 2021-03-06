#include "mocks.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using testing::Invoke;
using testing::Return;
using testing::ReturnNew;
using testing::ReturnRef;
using testing::SaveArg;
using testing::_;

namespace Envoy {
namespace Server {

MockOptions::MockOptions(const std::string& config_path, const std::string& bootstrap_path)
    : config_path_(config_path), bootstrap_path_(bootstrap_path), admin_address_path_("") {
  ON_CALL(*this, configPath()).WillByDefault(ReturnRef(config_path_));
  ON_CALL(*this, bootstrapPath()).WillByDefault(ReturnRef(bootstrap_path_));
  ON_CALL(*this, adminAddressPath()).WillByDefault(ReturnRef(admin_address_path_));
}
MockOptions::~MockOptions() {}

MockAdmin::MockAdmin() {}
MockAdmin::~MockAdmin() {}

MockDrainManager::MockDrainManager() {
  ON_CALL(*this, startDrainSequence(_)).WillByDefault(SaveArg<0>(&drain_sequence_completion_));
}
MockDrainManager::~MockDrainManager() {}

MockWatchDog::MockWatchDog() {}
MockWatchDog::~MockWatchDog() {}

MockGuardDog::MockGuardDog() : watch_dog_(new NiceMock<MockWatchDog>()) {
  ON_CALL(*this, createWatchDog(_)).WillByDefault(Return(watch_dog_));
}
MockGuardDog::~MockGuardDog() {}

MockHotRestart::MockHotRestart() {}
MockHotRestart::~MockHotRestart() {}

MockListenerComponentFactory::MockListenerComponentFactory()
    : socket_(std::make_shared<NiceMock<Network::MockListenSocket>>()) {
  ON_CALL(*this, createListenSocket(_, _)).WillByDefault(Return(socket_));
}
MockListenerComponentFactory::~MockListenerComponentFactory() {}

MockListenerManager::MockListenerManager() {}
MockListenerManager::~MockListenerManager() {}

MockListener::MockListener() {
  ON_CALL(*this, filterChainFactory()).WillByDefault(ReturnRef(filter_chain_factory_));
  ON_CALL(*this, socket()).WillByDefault(ReturnRef(socket_));
  ON_CALL(*this, listenerScope()).WillByDefault(ReturnRef(scope_));
  ON_CALL(*this, name()).WillByDefault(ReturnRef(name_));
}
MockListener::~MockListener() {}

MockWorkerFactory::MockWorkerFactory() {}
MockWorkerFactory::~MockWorkerFactory() {}

MockWorker::MockWorker() {
  ON_CALL(*this, addListener(_, _))
      .WillByDefault(Invoke([this](Listener&, AddListenerCompletion completion) -> void {
        EXPECT_EQ(nullptr, add_listener_completion_);
        add_listener_completion_ = completion;
      }));

  ON_CALL(*this, removeListener(_, _))
      .WillByDefault(Invoke([this](Listener&, std::function<void()> completion) -> void {
        EXPECT_EQ(nullptr, remove_listener_completion_);
        remove_listener_completion_ = completion;
      }));
}
MockWorker::~MockWorker() {}

MockInstance::MockInstance() : ssl_context_manager_(runtime_loader_) {
  ON_CALL(*this, threadLocal()).WillByDefault(ReturnRef(thread_local_));
  ON_CALL(*this, stats()).WillByDefault(ReturnRef(stats_store_));
  ON_CALL(*this, httpTracer()).WillByDefault(ReturnRef(http_tracer_));
  ON_CALL(*this, dnsResolver()).WillByDefault(Return(dns_resolver_));
  ON_CALL(*this, api()).WillByDefault(ReturnRef(api_));
  ON_CALL(*this, admin()).WillByDefault(ReturnRef(admin_));
  ON_CALL(*this, clusterManager()).WillByDefault(ReturnRef(cluster_manager_));
  ON_CALL(*this, sslContextManager()).WillByDefault(ReturnRef(ssl_context_manager_));
  ON_CALL(*this, accessLogManager()).WillByDefault(ReturnRef(access_log_manager_));
  ON_CALL(*this, runtime()).WillByDefault(ReturnRef(runtime_loader_));
  ON_CALL(*this, dispatcher()).WillByDefault(ReturnRef(dispatcher_));
  ON_CALL(*this, hotRestart()).WillByDefault(ReturnRef(hot_restart_));
  ON_CALL(*this, random()).WillByDefault(ReturnRef(random_));
  ON_CALL(*this, localInfo()).WillByDefault(ReturnRef(local_info_));
  ON_CALL(*this, options()).WillByDefault(ReturnRef(options_));
  ON_CALL(*this, drainManager()).WillByDefault(ReturnRef(drain_manager_));
  ON_CALL(*this, initManager()).WillByDefault(ReturnRef(init_manager_));
  ON_CALL(*this, listenerManager()).WillByDefault(ReturnRef(listener_manager_));
}

MockInstance::~MockInstance() {}

namespace Configuration {

MockMain::MockMain(int wd_miss, int wd_megamiss, int wd_kill, int wd_multikill)
    : wd_miss_(wd_miss), wd_megamiss_(wd_megamiss), wd_kill_(wd_kill), wd_multikill_(wd_multikill) {
  ON_CALL(*this, wdMissTimeout()).WillByDefault(Return(wd_miss_));
  ON_CALL(*this, wdMegaMissTimeout()).WillByDefault(Return(wd_megamiss_));
  ON_CALL(*this, wdKillTimeout()).WillByDefault(Return(wd_kill_));
  ON_CALL(*this, wdMultiKillTimeout()).WillByDefault(Return(wd_multikill_));
}

MockFactoryContext::MockFactoryContext() {
  ON_CALL(*this, accessLogManager()).WillByDefault(ReturnRef(access_log_manager_));
  ON_CALL(*this, clusterManager()).WillByDefault(ReturnRef(cluster_manager_));
  ON_CALL(*this, dispatcher()).WillByDefault(ReturnRef(dispatcher_));
  ON_CALL(*this, drainDecision()).WillByDefault(ReturnRef(drain_manager_));
  ON_CALL(*this, httpTracer()).WillByDefault(ReturnRef(http_tracer_));
  ON_CALL(*this, initManager()).WillByDefault(ReturnRef(init_manager_));
  ON_CALL(*this, localInfo()).WillByDefault(ReturnRef(local_info_));
  ON_CALL(*this, random()).WillByDefault(ReturnRef(random_));
  ON_CALL(*this, runtime()).WillByDefault(ReturnRef(runtime_loader_));
  ON_CALL(*this, scope()).WillByDefault(ReturnRef(scope_));
  ON_CALL(*this, server()).WillByDefault(ReturnRef(server_));
  ON_CALL(*this, threadLocal()).WillByDefault(ReturnRef(thread_local_));
}

MockFactoryContext::~MockFactoryContext() {}

} // namespace Configuration
} // namespace Server
} // namespace Envoy
