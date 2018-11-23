/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__
#define MBED_EXTENDED_CONNECT_PARAMETERS_H__

#include "ble/BLETypes.h"
#include "mbed_assert.h"

/**
 * @addtogroup ble
 * @{
 * @addtogroup gap
 * @{
 */

namespace ble {

/**
 * Parameters defining the connection initiation process.
 *
 * The connection initiation process is divided in two different phases. First
 * the initiating device scan for the peer it should connect. Once the peer has
 * been found, it sends a connection request that contains the connection
 * parameters.
 *
 * @par Scan parameters
 *
 * The scan parameters are defined by two durations: the scan interval and the
 * scan window. The scan interval is the duration between two scan cycle and the
 * scan window defines how long the device search during a scan cycle.
 *
 * The scan window and the scan interval can be set at construction time or by
 * calling setScanParameters().
 *
 * @par Connection parameters
 *
 * A Bluetooth connection is defined by three parameters:
 *   - Connection interval: The time between two connection events. A minimum
 *   and a maximum connection interval are requested to help the Bluetooth
 *   subsystem deal with concurrent radio processing.
 *   - Slave latency: Number of connection events that can be ignored by the
 *   slave.
 *   - Supervision timeout: Time after which the connection is considered lost
 *   if the connected devices haven't exchanged a single packet. It is important
 *   to note that even if the application doesn't send actual data, the Bluetooth
 *   controller takes care of sending empty data packets to maintain the
 *   connection alive.
 *
 * These parameters can be set at construction time or by calling the function
 * setConnectionParameters().
 *
 * @par Phy
 *
 * Bluetooth 5 have introduced the support of different physical layer to either
 * increase the range or the throughput. Multiple phy can be configured
 * independently for scanning and connecting.
 *
 * Legacy connection happens on the 1M phy (phy_t::LE_1M). It is the only phy
 * that can be configured on legacy systems.
 *
 * The constructor, setScanParameters() and setConnectionParameters() accept
 * a phy_t parameter that defines to which PHY the parameters set applies.
 *
 * @par Other parameters:
 *
 * It is possible to defined what type of address is used to establish the
 * connection and whether or not if the whitelist should be used to find the peer
 * to connect to.
 *
 * @par Example:
 *
 * Thanks to the fluent API it is easy to compose the connection parameters at
 * instantiation point:
 *
 * @code
 *
    void do_connect(ble::Gap& gap, ble::target_peer_address_type_t addr_type, ble::address_t& address)
    {
        using namespace ble;

        gap.connect(
            addr_type,
            address,
            ConnectionParameters()
                .setScanParameters(
                    phy_t::LE_1M,
                    scan_interval_t(millisecond_t(500)),
                    scan_window_t(millisecond_t(250))
                )
                .setConnectionParameters(
                    phy_t::LE_1M,
                    conn_interval_t(millisecond_t(100)),
                    conn_interval_t(millisecond_t(200)),
                    slave_latency_t(0),
                    supervision_timeout_t(millisecond_t(1000))
                )
                .setOwnAddressType(own_address_type_t::RANDOM)
        );
    }
 *
 * @endcode
 *
 * @note It is not possible to configure phy_t::LE_2M for scanning.
 *
 * @see ble::Gap::connect()
 */
class ConnectionParameters {
    static const uint8_t MAX_PARAM_PHYS = 3;
    static const uint8_t LE_1M_INDEX = 0;
    static const uint8_t LE_2M_INDEX = 1;
    static const uint8_t LE_CODED_INDEX = 2;

public:
    /**
     * Create a ConnectionParameters object.
     *
     * @param phy The phy being configured.
     * @param scanInterval Interval between two scans.
     * @param scanWindow Scan duration during a scan interval.
     * @param minConnectionInterval Minimum value of the connection interval.
     * @param maxConnectionInterval Maximum value of the connection interval.
     * @param slaveLatency Maximum number of packets the slave can drop.
     * @param connectionSupervisionTimeout Time after which the connection is
     * considered lost if no data exchanged have taken place.
     * @param minEventLength Minimum duration of a connection event.
     * @param maxEventLength Maximum duration of a connection event
     */
    ConnectionParameters(
        phy_t phy = phy_t::LE_1M,
        scan_interval_t scanInterval = scan_interval_t::min(),
        scan_window_t scanWindow = scan_window_t::min(),
        conn_interval_t minConnectionInterval = conn_interval_t::min(),
        conn_interval_t maxConnectionInterval = conn_interval_t::max(),
        slave_latency_t slaveLatency = slave_latency_t::min(),
        supervision_timeout_t connectionSupervisionTimeout = supervision_timeout_t::max(),
        conn_event_length_t minEventLength = conn_event_length_t::min(),
        conn_event_length_t maxEventLength = conn_event_length_t::max());

    /* setters */

    /**
     * Set the scan parameters for a given phy.
     *
     * @param phy PHY being configured.
     * @param scanInterval Interval between two scans.
     * @param scanWindow Scan duration within a scan interval.
     *
     * @note It is useless to configure the 2M phy as it is not used during
     * scanning.
     *
     * @return A reference to this.
     */
    ConnectionParameters &setScanParameters(
        phy_t phy,
        scan_interval_t scanInterval,
        scan_window_t scanWindow
    );

    /**
     * Set the conenction parameters of a given PHY.
     *
     * @param phy The PHY being configured.
     * @param minConnectionInterval Minimum connection interval.
     * @param maxConnectionInterval Maximum connection interval.
     * @param slaveLatency Maximum number of packets the slave can drop.
     * @param connectionSupervisionTimeout Time after which the connection is
     * considered lost if no data exchanged have taken place.
     * @param minEventLength Minimum duration of a connection event.
     * @param maxEventLength Maximum duration of a connection event.
     *
     * @return A reference to this.
     */
    ConnectionParameters &setConnectionParameters(
        phy_t phy,
        conn_interval_t minConnectionInterval,
        conn_interval_t maxConnectionInterval,
        slave_latency_t slaveLatency,
        supervision_timeout_t connectionSupervisionTimeout,
        conn_event_length_t minEventLength = conn_event_length_t::min(),
        conn_event_length_t maxEventLength = conn_event_length_t::max()
    );

    /**
     * Address type used by the local device to connect the peer.
     * @param ownAddress Type of address used to initiate the connection.
     * @return A reference to this.
     */
    ConnectionParameters &setOwnAddressType(own_address_type_t ownAddress)
    {
        _ownAddressType = ownAddress;
        return *this;
    }

    /**
     * Set if the whitelist should be used to find the peer.
     *
     * @param filterPolicy The initiator filter to apply.
     *
     * @return A reference to this.
     */
    ConnectionParameters &setFilterPolicy(initiator_filter_policy_t filterPolicy)
    {
        _filterPolicy = filterPolicy;

        return *this;
    }

    /**
     * Enable or disable phys.
     *
     * @param phy1M true to enable the 1M PHY and false to disable it.
     * @param phy2M true to enable the 2M PHY and false to disable it.
     * @param phyCoded true to enable the CODED PHY and false to disable it.
     *
     * @return A reference to this.
     */
    ConnectionParameters &togglePhy(bool phy1M, bool phy2M, bool phyCoded)
    {
        handlePhyToggle(phy_t::LE_1M, phy1M);
        handlePhyToggle(phy_t::LE_2M, phy2M);
        handlePhyToggle(phy_t::LE_CODED, phyCoded);

        return *this;
    }

    /**
     * Disable an individual phy.
     *
     * @param phy The phy to disable.
     *
     * @return A reference to this.
     */
    ConnectionParameters &disablePhy(phy_t phy = phy_t::LE_1M)
    {
        handlePhyToggle(phy, false);

        return *this;
    }

    /**
     * Enable an individual phy.
     *
     * @param phy The phy to enable.
     *
     * @return A reference to this.
     */
    ConnectionParameters &enablePhy(phy_t phy = phy_t::LE_1M)
    {
        handlePhyToggle(phy, true);

        return *this;
    }

    /* getters */

    /**
     * Return the local address type used.
     *
     * @return The local address type to use.
     */
    own_address_type_t getOwnAddressType() const
    {
        return _ownAddressType;
    }

    /**
     * Return the initiator policy.
     *
     * @return The initiator policy.
     */
    initiator_filter_policy_t getFilterPolicy() const
    {
        return _filterPolicy;
    }

    /**
     * Return the number of phy enabled.
     * @return The number of phy enabled.
     */
    uint8_t getNumberOfEnabledPhys() const
    {
        return (
            _enabledPhy[LE_1M_INDEX] * 1 +
            _enabledPhy[LE_2M_INDEX] * 1 +
            _enabledPhy[LE_CODED_INDEX] * 1
        );
    }

#if !defined(DOXYGEN_ONLY)

    uint8_t getPhySet() const
    {
        phy_set_t set(
            _enabledPhy[LE_1M_INDEX],
            _enabledPhy[LE_2M_INDEX],
            _enabledPhy[LE_CODED_INDEX]
        );
        return set.value();
    }

    /* these return pointers to arrays of settings valid only across the number of active PHYs */

    const uint16_t *getScanIntervalArray() const
    {
        return &_scanInterval[getFirstEnabledIndex()];
    }

    const uint16_t *getScanWindowArray() const
    {
        return &_scanWindow[getFirstEnabledIndex()];
    }

    const uint16_t *getMinConnectionIntervalArray() const
    {
        return &_minConnectionInterval[getFirstEnabledIndex()];
    }

    const uint16_t *getMaxConnectionIntervalArray() const
    {
        return &_maxConnectionInterval[getFirstEnabledIndex()];
    }

    const uint16_t *getSlaveLatencyArray() const
    {
        return &_slaveLatency[getFirstEnabledIndex()];
    }

    const uint16_t *getConnectionSupervisionTimeoutArray() const
    {
        return &_connectionSupervisionTimeout[getFirstEnabledIndex()];
    }

    const uint16_t *getMinEventLengthArray() const
    {
        return &_minEventLength[getFirstEnabledIndex()];
    }

    const uint16_t *getMaxEventLengthArray() const
    {
        return &_maxEventLength[getFirstEnabledIndex()];
    }

#endif

private:
    uint8_t getFirstEnabledIndex() const
    {
        if (_enabledPhy[LE_1M_INDEX]) {
            return 0;
        } else if (_enabledPhy[LE_2M_INDEX]) {
            return 1;
        } else if (_enabledPhy[LE_CODED_INDEX]) {
            return 2;
        }
        /* this should never happen, it means you were trying to start a connection with a blank set
         * of paramters - you need to enabled at least one phy */
        MBED_ASSERT("Trying to use connection parameters without any PHY defined.");
        return 0;
    }

    /** Handle toggling PHYs on and off and return the correct index to use to set the configuration elements.
     *
     * @param phy Which Phy is being toggle.
     * @param enable On or Off.
     * @return The index to the array of settings.
     */
    uint8_t handlePhyToggle(phy_t phy, bool enable)
    {
        uint8_t index = phyToIndex(phy);

        bool was_swapped = false;
        bool is_swapped = false;

        if (isSwapped()) {
            was_swapped = true;
        }

        _enabledPhy[index] = enable;

        if (isSwapped()) {
            is_swapped = true;
        }

        if (was_swapped != is_swapped) {
            swapCodedAnd2M();
        }

        if (is_swapped && phy == phy_t::LE_CODED) {
            index -= 1;
        }

        return index;
    }

    uint8_t phyToIndex(phy_t phy) const
    {
        uint8_t index;
        switch(phy.value()) {
            case phy_t::LE_1M:
                index = 0;
                break;
            case phy_t::LE_2M:
                index = 1;
                break;
            case phy_t::LE_CODED:
                index = 2;
                break;
            default:
                index = MAX_PARAM_PHYS;
                MBED_ASSERT("Illegal PHY");
                break;
        }
        return index;
    }

    bool isSwapped() const
    {
        return (
            _enabledPhy[LE_1M_INDEX] &&
            !_enabledPhy[LE_2M_INDEX] &&
            _enabledPhy[LE_CODED_INDEX]
        );
    }

    /** Handle the swapping of 2M and CODED so that the array is ready for the pal call. */
    void swapCodedAnd2M();

private:
    initiator_filter_policy_t _filterPolicy;
    own_address_type_t _ownAddressType;

    uint16_t _scanInterval[MAX_PARAM_PHYS]; /* 0.625 ms */
    uint16_t _scanWindow[MAX_PARAM_PHYS]; /* 0.625 ms */
    uint16_t _minConnectionInterval[MAX_PARAM_PHYS]; /* 1.25 ms */
    uint16_t _maxConnectionInterval[MAX_PARAM_PHYS]; /* 1.25 ms */
    uint16_t _slaveLatency[MAX_PARAM_PHYS]; /* events */
    uint16_t _connectionSupervisionTimeout[MAX_PARAM_PHYS]; /* 10 ms */
    uint16_t _minEventLength[MAX_PARAM_PHYS]; /* 0.625 ms */
    uint16_t _maxEventLength[MAX_PARAM_PHYS]; /* 0.625 ms */

    bool _enabledPhy[MAX_PARAM_PHYS];
};

} // namespace ble

/**
 * @}
 * @}
 */

#endif /* ifndef MBED_EXTENDED_CONNECT_PARAMETERS_H__ */
