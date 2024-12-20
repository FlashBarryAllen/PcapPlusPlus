#pragma once

#include "Layer.h"

/// @file

/**
 * \namespace pcpp
 * \brief The main namespace for the PcapPlusPlus lib
 */
namespace pcpp
{
	/**
	 * @class InfiniBandLayer
	 * Represents an InfiniBand protocol layer
	 */
	class InfiniBandLayer : public Layer
	{
	private:
		/**
		 * @struct bth
		 * Represents an Base Transport Header
		 */
#pragma pack(push, 1)
		struct rxe_bth
		{
			uint8_t opcode;
			uint8_t flags;
			uint16_t pkey;
			uint32_t qpn;
			uint32_t apsn;
		};
#pragma pack(pop)

	public:
		/**
		 * A constructor that creates the layer from an existing packet raw data
		 * @param[in] data A pointer to the raw data (will be casted to bth_header)
		 * @param[in] dataLen Size of the data in bytes
		 * @param[in] prevLayer A pointer to the previous layer
		 * @param[in] packet A pointer to the Packet instance where layer will be stored in
		 */
		InfiniBandLayer(uint8_t* data, size_t dataLen, Layer* prevLayer, Packet* packet)
		    : Layer(data, dataLen, prevLayer, packet, InfiniBand)
		{}

		/**
		 * A constructor that creates a new rxe_bth header and allocates the data
		 * @param[in] opcode The operation code
		 * @param[in] soliciteEvent The solicited event
		 * @param[in] migrationState The migration state
		 * @param[in] padCount The pad count
		 * @param[in] partitionKey The partition key
		 * @param[in] queuePairNumber The destination queue pair (QP) number
		 * @param[in] ackReq The acknowledgment request
		 * @param[in] packetSequenceNumber The packet sequence number
		 */
		InfiniBandLayer(uint8_t opcode, int soliciteEvent, int migrationState, int padCount, uint16_t partitionKey,
						uint32_t queuePairNumber, int ackReq, uint32_t packetSequenceNumber);

		/**
		 * @return The operation code which defines the interpretation of the remaining header and payload bytes
		 */
		uint8_t getOpcode() const;

		/**
		 * Set operation code
		 * @param[in] opcode The opcode to set
		 */
		void setOpcode(uint8_t opcode) const;

		/**
		 * @return solicited event that the responder shall invoke the CQ event handler
		 */
		bool getSoliciteEvent() const;

		/**
		 * Set solicited event
		 * @param[in] se The solicited event to set
		 */
		void setSolicitedEvent(bool se) const;

		/**
		 * @return migreq which used to communicate migration state
		 */
		bool getMigrationState() const;

		/**
		 * Set migreq
		 * @param[in] mig The migration state to set. If set to one, indicates the connection or EE context has been
		 * migrated; if set to zero, it means there is no change in the current migration state.
		 */
		void setMigrationState(bool mig) const;

		/**
		 * @return PadCount which Packet payloads are sent as a multiple of 4-byte quantities.
		 * Pad count indicates the number of pad bytes - 0 to 3 - that are appended to the packetpayload.
		 * Pads are used to “stretch” the payload (payloads may be zero or more bytes in length) to be a multiple of 4
		 * bytes
		 */
		uint8_t getPadCount() const;

		/**
		 * Set PadCount
		 * @param[in] pad The PadCount to set
		 */
		void setPadCount(uint8_t pad) const;

		/**
		 * @return Transport Header Version that specifies the version of the IBA Transport used for this packet
		 */
		uint8_t getTransportHeaderVersion() const;

		/**
		 * Set Transport Header Version
		 * @param[in] tvr The transport header version to set
		 */
		void setTransportHeaderVersion(uint8_t tver) const;

		/**
		 * @return partition key identifying the partition
		 * that the destination QP (RC, UC, UD, XRC) or EE Context (RD) is a member.
		 */
		uint16_t getPartitionKey() const;

		/**
		 * Set partition key
		 * @param[in] pkey The partition key to set
		 */
		void setPartitionKey(uint16_t pkey) const;

		/**
		 * @return destination queue pair (QP) identifier
		 */
		uint32_t getQueuePairNumber() const;

		/**
		 * Set Queue Pair Number
		 * @param[in] qpn The queue pair number to set
		 */
		void setQueuePairNumber(uint32_t qpn) const;

		/**
		 * @return FECN
		 * F (FECN): false indicates that a FECN indication was not received.
		 * true indicates that the packet went through a point of congestion
		 */
		bool getFecn() const;

		/**
		 * Set Fecn
		 * @param[in] fecn The FECN to set
		 */
		void setFecn(bool fecn) const;

		/**
		 * @return BECN
		 * B (BECN): false the packet did not go through a point of congestion or went
		 * through a point of congestion but was not marked. true indicates that the
		 * packet indicated by this header was subject to forward congestion. The B
		 * bit is set in an ACK or CN BTH
		 */
		bool getBecn() const;

		/**
		 * Set BECN
		 * @param[in] becn The BECN to set
		 */
		void setBecn(bool becn) const;

		/**
		 * Set Reserved 6 bits
		 */
		void setResv6a() const;

		/**
		 * @return ackreq that requests responder to schedule an acknowledgment on the associated QP.
		 */
		int getAck() const;

		/**
		 * Set acknowledgment for requests
		 * @param[in] ack The acknowledgment to set
		 */
		void setAck(int ack) const;

		/**
		 * @return packet sequence number that is used to identify the position of a packet
		 * within a sequence of packets.
		 */
		uint32_t getPacketSequenceNumber() const;

		/**
		 * Set packet sequence number
		 * @param[in] psn The packet sequence number to set
		 */
		void setPacketSequenceNumber(uint32_t psn) const;

		/**
		 * Identify the next layer as PayloadLayer
		 */
		void parseNextLayer() override;

		/**
		 * @return Size of rxe_bth header
		 */
		size_t getHeaderLen() const override
		{
			return sizeof(rxe_bth);
		}

		/**
		 * Does nothing for this layer
		 */
		void computeCalculateFields() override
		{}

		std::string toString() const override;

		OsiModelLayer getOsiModelLayer() const override
		{
			return OsiModelTransportLayer;
		}

		/**
		 * A static method that check whether is inifiniband RoCE port
		 * @param[in] port The port from UDP destination port
		 * @return True if the port is inifiniband RoCE and can represent an rxe packet
		 */
		static inline bool isInfiniBandPort(uint16_t port)
		{
			return (port == 4791);
		}

		/**
		 * The static method makes validation of UDP data
		 * @param[in] udpData The pointer to the UDP payload data. It points to the first byte of rxe_bth header.
		 * @param[in] udpDataLen The payload data size
		 * @return True if the data is valid and can represent the rxe_bth packet
		 */
		static bool isDataValid(const uint8_t* udpData, size_t udpDataLen);

	private:
		/**
		 * Get a pointer to the BTH header. Notice this points directly to the data, so every change will change
		 * the actual packet data
		 * @return A pointer to the bth_header
		 */
		rxe_bth* getBthHeader() const
		{
			return reinterpret_cast<rxe_bth*>(m_Data);
		}
	};

}  // namespace pcpp
