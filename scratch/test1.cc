// Network topology
//
//    SRC
//     |<=== source network
//     A-----B
//      \   / \   all networks have cost 1, except
//       \ /  |   for the direct link from C to D, which
//        C  /    has cost 10
//        | /
//        |/
//        D
//        |<=== target network
//       DST
//
//
// A, B, C and D are RIPng routers.
// A and D are configured with static addresses.
// SRC and DST will exchange packets.
//
// After about 3 seconds, the topology is built, and Echo Reply will be received.
// After 40 seconds, the link between B and D will break, causing a route failure.
// After 44 seconds from the failure, the routers will recovery from the failure.
// Split Horizoning should affect the recovery time, but it is not. See the manual
// for an explanation of this effect.



#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/traffic-control-module.h"
#include "ns3/applications-module.h"

#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test1");
void
ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;

  while ((packet = socket->RecvFrom (from)))
    {
	  std::cout << from << std::endl;
      if (packet->GetSize () > 0)
        {
    	  Inet6SocketAddress iaddr = Inet6SocketAddress::ConvertFrom (from);

    	    std::ostringstream oss;
    	    oss << "Received one packet! Socket: " << iaddr.GetIpv6 ()
    	        << " port: " << iaddr.GetPort ()
    	        << " at time = " << Simulator::Now ().GetSeconds ()
    	        << "";

    	  std::string x = oss.str();
         // NS_LOG_UNCOND (x);
        }
    }
}

void
AddListener(Ptr<Node> node, uint16_t port)
{

    // TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

    Ptr<Socket> recvSink = Socket::CreateSocket (node, tid);  // node 1, receiver
    Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (), port);
    recvSink->Bind (local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

}


int main (int argc, char **argv)
{
	bool printRoutingTables = false;
	bool showPings = false;

#if 1

	if (showPings)
	{
		LogComponentEnable ("Ping6Application", LOG_LEVEL_INFO);
	}
	NS_LOG_INFO ("Create nodes.");
	Ptr<Node> src = CreateObject<Node> ();
	Names::Add ("SrcNode", src);
	Ptr<Node> dst = CreateObject<Node> ();
	Names::Add ("DstNode", dst);
	Ptr<Node> a = CreateObject<Node> ();
	Names::Add ("RouterA", a);
	Ptr<Node> b = CreateObject<Node> ();
	Names::Add ("RouterB", b);
	Ptr<Node> c = CreateObject<Node> ();
	Names::Add ("RouterC", c);
	Ptr<Node> d = CreateObject<Node> ();
	Names::Add ("RouterD", d);
	NodeContainer net1 (src, a);
	NodeContainer net2 (a, b);
	NodeContainer net3 (a, c);
	NodeContainer net4 (b, c);
	NodeContainer net5 (c, d);
	NodeContainer net6 (b, d);
	NodeContainer net7 (d, dst);
	NodeContainer routers (a, b, c, d);
	NodeContainer nodes (src, dst);

	NS_LOG_INFO ("Create channels.");
	CsmaHelper csma;
	csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
	NetDeviceContainer ndc1 = csma.Install (net1);
	NetDeviceContainer ndc2 = csma.Install (net2);
	NetDeviceContainer ndc3 = csma.Install (net3);
	NetDeviceContainer ndc4 = csma.Install (net4);
	NetDeviceContainer ndc5 = csma.Install (net5);
	NetDeviceContainer ndc6 = csma.Install (net6);
	NetDeviceContainer ndc7 = csma.Install (net7);

	NS_LOG_INFO ("Create IPv6 and routing");
	RipNgHelper ripNgRouting;
	// Rule of thumb:
	// Interfaces are added sequentially, starting from 0
	// However, interface 0 is always the loopback...
	ripNgRouting.ExcludeInterface (a, 1);
	ripNgRouting.ExcludeInterface (d, 3);
	ripNgRouting.SetInterfaceMetric (c, 3, 10);
	ripNgRouting.SetInterfaceMetric (d, 1, 10);

	Ipv6ListRoutingHelper listRH;
	listRH.Add (ripNgRouting, 0);
	Ipv6StaticRoutingHelper staticRh;
	listRH.Add (staticRh, 5);

	InternetStackHelper internetv6;
	internetv6.SetIpv4StackInstall (false);
	internetv6.SetRoutingHelper (listRH);
	internetv6.Install (routers);

	InternetStackHelper internetv6Nodes;
	internetv6Nodes.SetIpv4StackInstall (false);
	internetv6Nodes.Install (nodes);

	// Assign addresses.
	// The source and destination networks have global addresses
	// The "core" network just needs link-local addresses for routing.
	// We assign global addresses to the routers as well to receive
	// ICMPv6 errors.
	NS_LOG_INFO ("Assign IPv6 Addresses.");
	Ipv6AddressHelper ipv6;

	ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic1 = ipv6.Assign (ndc1);
	iic1.SetForwarding (1, true);
	iic1.SetDefaultRouteInAllNodes (1);

	ipv6.SetBase (Ipv6Address ("2001:0:1::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic2 = ipv6.Assign (ndc2);
	iic2.SetForwarding (0, true);
	iic2.SetForwarding (1, true);

	ipv6.SetBase (Ipv6Address ("2001:0:2::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic3 = ipv6.Assign (ndc3);
	iic3.SetForwarding (0, true);
	iic3.SetForwarding (1, true);

	ipv6.SetBase (Ipv6Address ("2001:0:3::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic4 = ipv6.Assign (ndc4);
	iic4.SetForwarding (0, true);
	iic4.SetForwarding (1, true);

	ipv6.SetBase (Ipv6Address ("2001:0:4::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic5 = ipv6.Assign (ndc5);
	iic5.SetForwarding (0, true);
	iic5.SetForwarding (1, true);

	ipv6.SetBase (Ipv6Address ("2001:0:5::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic6 = ipv6.Assign (ndc6);
	iic6.SetForwarding (0, true);
	iic6.SetForwarding (1, true);

	ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer iic7 = ipv6.Assign (ndc7);
	iic7.SetForwarding (0, true);
	iic7.SetDefaultRouteInAllNodes (0);

	if (printRoutingTables)
	    {
	      RipNgHelper routingHelper;
	      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
	      routingHelper.PrintRoutingTableAt (Seconds (30.0), a, routingStream);
		  routingHelper.PrintRoutingTableAt (Seconds (30.0), b, routingStream);
		  routingHelper.PrintRoutingTableAt (Seconds (30.0), c, routingStream);
		  routingHelper.PrintRoutingTableAt (Seconds (30.0), d, routingStream);
	    }


	NS_LOG_INFO ("Create Applications.");

	Ping6Helper ping6;

	ping6.SetLocal (iic1.GetAddress (0, 1));
	ping6.SetRemote (iic7.GetAddress (1, 1));

	ping6.SetAttribute ("MaxPackets", UintegerValue (100));
	ping6.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	ping6.SetAttribute ("PacketSize", UintegerValue (1024));
	ApplicationContainer app_ping6 = ping6.Install (src);
	app_ping6.Start (Seconds (10.0));
	app_ping6.Stop (Seconds (20.0));


//	UdpClientHelper client (Ipv6Address ("2001:2::1"), 8000);
//	client.SetAttribute ("MaxPackets", UintegerValue (320));
//	client.SetAttribute ("Interval", TimeValue (Seconds (0.05)));
//	client.SetAttribute ("PacketSize", UintegerValue (1024));
//	ApplicationContainer app_udpClient = client.Install (a);
//
//	app_udpClient.Start (Seconds (11.0));
//	app_udpClient.Stop (Seconds (19.0));
//
//	UdpServerHelper server (8000);
//	ApplicationContainer app_udpServer = server.Install (dst);
//
//	app_udpServer.Start (Seconds (10.0));
//	app_udpServer.Stop (Seconds (20.0));
#endif

//
//	  //Receiver socket on n1
//
//
//    uint16_t port = 9;   // Discard port (RFC 863)
//
//    OnOffHelper onoff ("ns3::UdpSocketFactory",
//                       Inet6SocketAddress (iic7.GetAddress (1, 1), port));
//    onoff.SetConstantRate (DataRate ("2kbps"));
//    onoff.SetAttribute ("PacketSize", UintegerValue (100));
//
//    ApplicationContainer apps = onoff.Install (src);
//    apps.Start (Seconds (20.0));
//    apps.Stop (Seconds (30.0));
//
//
//    AddListener(c,9);
























	AsciiTraceHelper ascii;
	csma.EnableAsciiAll (ascii.CreateFileStream ("test1.tr"));
	csma.EnablePcapAll ("test1", true);

	/* Now, do the actual simulation. */
	NS_LOG_INFO ("Run Simulation.");
	Simulator::Stop (Seconds (120));
	Simulator::Run ();
	Simulator::Destroy ();
	NS_LOG_INFO ("Done.");







}

