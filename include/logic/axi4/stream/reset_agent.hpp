/* Copyright 2018 Tymoteusz Blazejczyk
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

#ifndef LOGIC_AXI4_STREAM_RESET_AGENT_HPP
#define LOGIC_AXI4_STREAM_RESET_AGENT_HPP

#include <uvm>

namespace logic {
namespace axi4 {
namespace stream {

class packet;
class reset_driver;
class reset_sequencer;

class reset_agent : public uvm::uvm_agent {
public:
    UVM_COMPONENT_UTILS(logic::axi4::stream::reset_agent)

    reset_sequencer* sequencer;

    reset_agent();

    reset_agent(const uvm::uvm_component_name& name);

    virtual ~reset_agent() override;
protected:
    reset_agent(const reset_agent&) = delete;

    reset_agent& operator=(const reset_agent&) = delete;

    virtual void build_phase(uvm::uvm_phase& phase) override;

    virtual void connect_phase(uvm::uvm_phase& phase) override;

    reset_driver* m_driver;
};

}
}
}

#endif /* LOGIC_AXI4_STREAM_RESET_AGENT_HPP */
