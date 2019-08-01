#include <iostream>
#include <typeinfo>
#include "ns3/core-module.h"

#include "ns3/ipv6-header.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/udp-header.h"
#include "ns3/packet.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test3");

int main()
{

	LogComponentEnable ("Packet", LOG_LEVEL_ALL);

	RngSeedManager::SetSeed(1);
	Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable>();

	Ipv6Address ad("2001:1::200:ff:fe00:2");

	uint8_t buf[16];
	ad.GetBytes (buf);


	Ptr<Packet> p = Create<Packet> (buf, 128);




	UdpHeader hdr;
	hdr.SetDestinationPort(10);
	hdr.SetSourcePort(10);

	p->AddHeader(hdr);
	std::cout<< p->GetSize();

	// p->Print(std::cout);

	// hdr.Print(std::cout);

 return 0;
}
