#include "ara/core/initialization.h"
#include <ara/exec/state_client.h>
#include "ara/exec/execution_client.h"
#include <thread>

int main(int argc, char *argv[])
{
    ara::core::Initialize();
    
    // TODO: your code here
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);

	ara::core::Result<ara::exec::FunctionGroup::CtorToken> fgResult = ara::exec::FunctionGroup::Preconstruct("DeepRacerFG");
    ara::exec::FunctionGroup::CtorToken fgToken(fgResult.ValueOrThrow());   
	ara::exec::FunctionGroup fg(std::move(fgToken));
    
    const ara::core::String stateStr =  "DeepRacerFG/Simulator";
    ara::exec::StateClient stateClient(nullptr);
	ara::core::Result<ara::exec::FunctionGroupState::CtorToken> fgsResult = ara::exec::FunctionGroupState::Preconstruct(fg, stateStr);
	ara::exec::FunctionGroupState::CtorToken fgsToken(fgsResult.ValueOrThrow());
	ara::exec::FunctionGroupState fgs(std::move(fgsToken));

    stateClient.SetState(fgs);
 
    while(true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    ara::core::Deinitialize();
    
    return 0;
}
