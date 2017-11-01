/* Copyright 2017 Tymoteusz Blazejczyk
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

#include "logic/axi4/stream/test.hpp"

#include "logic/axi4/stream/sequence.hpp"
#include "logic/axi4/stream/sequencer.hpp"
#include "logic/axi4/stream/testbench.hpp"
#include "logic/axi4/stream/scoreboard.hpp"
#include "logic/axi4/stream/rx_sequencer.hpp"

using logic::axi4::stream::test;

test::test() :
    test{"test"}
{ }

test::test(const uvm::uvm_component_name& name) :
    uvm::uvm_test{name},
    m_sequence{nullptr},
    m_testbench{nullptr},
    m_test_passed{false}
{ }

test::~test() { }

void test::build_phase(uvm::uvm_phase& phase) {
    uvm::uvm_test::build_phase(phase);

    m_testbench = testbench::type_id::create("testbench", this);
    if (!m_testbench) {
        UVM_FATAL(get_name(), "Cannot create testbench!"
                " Simulation aborted!");
    }

    m_sequence = sequence::type_id::create("sequence", this);
    if (!m_sequence) {
        UVM_FATAL(get_name(), "Cannot create sequence!"
                " Simulation aborted!");
    }

    /*
    uvm::uvm_config_db<scv_smart_ptr<std::size_t>>::get(
        this, "", "reset_duration", m_sequence->reset_duration);
    */

    scv_random::set_global_seed(scv_random::pick_random_seed());

    m_sequence->reset_repeats->keep_only(1);
    m_sequence->reset_duration->keep_only(1);
    m_sequence->reset_idle->keep_only(0);
    m_sequence->rx_number_of_packets->keep_only(1);
    m_sequence->rx_repeats->keep_only(1);
    m_sequence->rx_packet_length->keep_only(1, 256);
    m_sequence->rx_idle_scheme->keep_only(0);
    m_sequence->tx_idle_scheme->keep_only(0);
}

void test::run_phase(uvm::uvm_phase& phase) {
    phase.raise_objection(this);

    m_sequence->start(m_testbench->sequencer);

    phase.drop_objection(this);
}

void test::extract_phase(uvm::uvm_phase& phase) {
    uvm::uvm_test::extract_phase(phase);

    m_test_passed = m_testbench->passed();
}

void test::report_phase(uvm::uvm_phase& phase) {
    uvm::uvm_test::report_phase(phase);

    if (m_test_passed) {
        UVM_INFO(get_name(), "TEST PASSED", uvm::UVM_FULL);
    }
    else {
        UVM_ERROR(get_name(), "TEST FAILED");
    }

    uvm::uvm_root::get()->finish_on_completion(false);
    uvm::uvm_report_server::get_server()->summarize();
    sc_core::sc_stop();
}