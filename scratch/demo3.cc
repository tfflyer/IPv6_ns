
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/header.h"
#include "ns3/ipv6-address.h"
#include "ns3/packet.h"
#include "ns3/ipv6-header.h"
#include "ns3/mac48-address.h"
#include "ns3/ethernet-header.h"
#include "ns3/ipv6-routing-protocol.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/mpi-interface.h"
#include "ns3/socket.h"
#include "ns3/callback.h"
#include "ns3/applications-module.h"
#include "ns3/ping6-helper.h"
#include "ns3/ripng.h"
#include "ns3/udp-header.h"
#include "ns3/flow-monitor-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test2");




int main (int argc, char **argv)
{
//    LogComponentEnable ("Ping6Example", LOG_LEVEL_INFO);
//    LogComponentEnable ("Ipv6EndPointDemux", LOG_LEVEL_ALL);
//    LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_ALL);
//    LogComponentEnable ("Ipv6StaticRouting", LOG_LEVEL_ALL);
//    LogComponentEnable ("Ipv6ListRouting", LOG_LEVEL_ALL);
    LogComponentEnable ("Ipv6Interface", LOG_LEVEL_INFO);
//    LogComponentEnable ("Icmpv6L4Protocol", LOG_LEVEL_ALL);
//    LogComponentEnable ("Ping6Application", LOG_LEVEL_INFO);;
//	  LogComponentEnable ("ping6ServerApplication", LOG_LEVEL_INFO);
//	LogComponentEnable ("Packet", LOG_LEVEL_INFO);
//	LogComponentEnable ("Ipv6-l3-protocol", LOG_LEVEL_INFO);

        // LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_ALL);
		CommandLine cmd;
		cmd.Parse (argc,argv);

		Ptr<Node> src = CreateObject<Node> ();
		Names::Add ("SrcNode", src);
		Ptr<Node> dst = CreateObject<Node> ();
		Names::Add ("DstNode", dst);
		Ptr<Node> route1 = CreateObject<Node>();
		Names::Add ("RouteNode1", route1);
		Ptr<Node> route2 = CreateObject<Node>();
		Names::Add ("RouteNode2", route2);
		Ptr<Node> route3 = CreateObject<Node>();
		Names::Add ("RouteNode3", route3);
		Ptr<Node> route4 = CreateObject<Node>();
		Names::Add ("RouteNode4", route4);
		Ptr<Node> route5 = CreateObject<Node>();
		Names::Add ("RouteNode5", route5);
		Ptr<Node> route6 = CreateObject<Node>();
		Names::Add ("RouteNode6", route6);
		Ptr<Node> route7 = CreateObject<Node>();
		Names::Add ("RouteNode7", route7);
		Ptr<Node> route8 = CreateObject<Node>();
		Names::Add ("RouteNode8", route8);
		Ptr<Node> route9 = CreateObject<Node>();
		Names::Add ("RouteNode9", route9);
		Ptr<Node> route10 = CreateObject<Node>();
		Names::Add ("RouteNode10", route10);

		NodeContainer net1 (src, route1);
		NodeContainer net2 (route1, route2);
		NodeContainer net3 (route2, route3);
		NodeContainer net4 (route3, route4);
		NodeContainer net5 (route4, route5);
		NodeContainer net6 (route5, route6);
		NodeContainer net7 (route6, route7);
		NodeContainer net8 (route7, route8);
		NodeContainer net9 (route8, route9);
		NodeContainer net10 (route9, route10);
		NodeContainer net11 (route10, dst);
		NodeContainer routers (route1, route2, route3,route4,route5);
		NodeContainer routers1(route6, route7, route8 ,route9,route10);
		NodeContainer nodes (src, dst);



		CsmaHelper csma;
		csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
		csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
		NetDeviceContainer ndc1 = csma.Install (net1);
		NetDeviceContainer ndc2 = csma.Install (net2);
		NetDeviceContainer ndc3 = csma.Install (net3);
		NetDeviceContainer ndc7 = csma.Install (net7);
		NetDeviceContainer ndc8 = csma.Install (net8);
		NetDeviceContainer ndc9 = csma.Install (net9);
		NetDeviceContainer ndc10 = csma.Install (net10);
		NetDeviceContainer ndc11 = csma.Install (net11);

//		CsmaHelper csma1;
//		csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
//		csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (3)));
		NetDeviceContainer ndc4 = csma.Install (net4);
		NetDeviceContainer ndc5 = csma.Install (net5);
		NetDeviceContainer ndc6 = csma.Install (net6);



		RipNgHelper ripNgRouting;
		ripNgRouting.ExcludeInterface (route1, 1);
		ripNgRouting.ExcludeInterface (route10, 2);



		Ipv6ListRoutingHelper listRH;
		listRH.Add (ripNgRouting, 0);
		Ipv6StaticRoutingHelper staticRh;
		listRH.Add (staticRh, 2);


		InternetStackHelper internetv6;
		internetv6.SetIpv4StackInstall (false);
		internetv6.SetRoutingHelper (listRH);
		internetv6.Install (routers);
		internetv6.Install (routers1);


		InternetStackHelper internetv6Nodes;
		internetv6Nodes.SetIpv4StackInstall (false);
		internetv6Nodes.Install (nodes);

		Ipv6AddressHelper ipv6;

		ipv6.SetBase (Ipv6Address ("2001:1::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic1 = ipv6.Assign (ndc1);
		iic1.SetForwarding (1, true);
		iic1.SetDefaultRouteInAllNodes (1);

		ipv6.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic2 = ipv6.Assign (ndc2);
		iic2.SetForwarding (0, true);
		iic2.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:3::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic3 = ipv6.Assign (ndc3);
		iic3.SetForwarding (0, true);
		iic3.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:4::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic4 = ipv6.Assign (ndc4);
		iic4.SetForwarding (0, true);
		iic4.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:5::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic5 = ipv6.Assign (ndc5);
		iic5.SetForwarding (0, true);
		iic5.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:6::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic6 = ipv6.Assign (ndc6);
		iic6.SetForwarding (0, true);
		iic6.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:7::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic7 = ipv6.Assign (ndc7);
		iic7.SetForwarding (0, true);
		iic7.SetForwarding (1, true);
//		iic7.SetDefaultRouteInAllNodes (0);

		ipv6.SetBase (Ipv6Address ("2001:8::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic8 = ipv6.Assign (ndc8);
		iic8.SetForwarding (0, true);
		iic8.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:9::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic9 = ipv6.Assign (ndc9);
		iic9.SetForwarding (0, true);
		iic9.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:10::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic10 = ipv6.Assign (ndc10);
		iic10.SetForwarding (0, true);
		iic10.SetForwarding (1, true);


		ipv6.SetBase (Ipv6Address ("2001:11::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic11 = ipv6.Assign (ndc11);
		iic11.SetForwarding (0, true);
//		iic10.SetForwarding (1, true);
		iic11.SetDefaultRouteInAllNodes (0);


//#if 0  // ping6

//		Ping6Helper ping6;
//		ping6.SetLocal (iic1.GetAddress (0, 1));
//		ping6.SetRemote (iic11.GetAddress (1, 1));
//
//		ping6.SetAttribute ("MaxPackets", UintegerValue (100));
//		ping6.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
//		ping6.SetAttribute ("PacketSize", UintegerValue (20));
//		ApplicationContainer app_ping6 = ping6.Install (src);
//		app_ping6.Start (Seconds (50.0));
//		app_ping6.Stop (Seconds (90.0));




		BulkSendHelper source("ns3::TcpSocketFactory",
				Inet6SocketAddress(iic11.GetAddress(1,1),34));
		source.SetAttribute("SendSize",UintegerValue(1024));
		ApplicationContainer sourceApps = source.Install(src);
		sourceApps.Start(Seconds(30));
		sourceApps.Stop(Seconds(120));


		PacketSinkHelper sink("ns3::TcpSocketFactory",
				Inet6SocketAddress(Ipv6Address::GetAny(),34));
		ApplicationContainer sinkApps = sink.Install(dst);
		sinkApps.Start(Seconds(30));
		sinkApps.Stop(Seconds(120));





//		UdpClientHelper client (Ipv6Address ("2001:1::1"), 8000);
//		client.SetAttribute ("MaxPackets", UintegerValue (320));
//		client.SetAttribute ("Interval", TimeValue (Seconds (1)));
//		client.SetAttribute ("PacketSize", UintegerValue (20));
//		ApplicationContainer app_udpClient = client.Install (src);

//		app_udpClient.Start (Seconds (2.0));
//		app_udpClient.Stop (Seconds (19.0));

//		UdpServerHelper server (8000);
//		ApplicationContainer app_udpServer = server.Install (dst);

//		app_udpServer.Start (Seconds (1.0));
//		app_udpServer.Stop (Seconds (20.0));

//#endif
//
//#if 0 // the address of router
//
//	      Ptr<NetDevice> device = ndc.Get(1) ;
//	      Ptr<Node> node = device->GetNode ();
//	      Ptr<Ipv6> i6 = node->GetObject<Ipv6> ();
//	      // i6 ->GetAddress(1,1);
//	      int32_t interface = i6->GetInterfaceForDevice (device);
//	      std :: cout << interface ;
//#endif
//
//#if 1// onoff application
//
//	      uint16_t port = 10;   // Discard port (RFC 863)
//
//	      OnOffHelper onoff ("ns3::UdpSocketFactory",
//	                         Inet6SocketAddress (iic11.GetAddress (1, 1), port));
//
//	      onoff.SetConstantRate (DataRate ("20kbps"));
//	      onoff.SetAttribute ("PacketSize", UintegerValue (100));
//
//	      ApplicationContainer apps = onoff.Install (src);
//
//	      apps.Start (Seconds (11.0));
//	      apps.Stop (Seconds (80.0));
//



	      // Ipv4GlobalRoutingHelper g;
	      // Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("dynamic-global-routing.routes", std::ios::out);
//	      // g.PrintRoutingTableAllAt (Seconds (12), routingStream);
//#endif
//
//#if 0 // udp server
//	      uint16_t port_updserver = 55;
//	      UdpServerHelper server (port_updserver);
//	      ApplicationContainer apps_udpserver = server.Install (dst);
//	      apps_udpserver.Start (Seconds (30.0));
//	      apps_udpserver.Stop (Seconds (41.0));
//#endif
//
//#if 0 // trace server
//	      uint16_t traceport = 3080;  // well-known echo port number
//	      UdpServerHelper traceserver (traceport);
//	      ApplicationContainer apps_traceserver = traceserver.Install (dst);
//	      apps_traceserver.Start (Seconds (0.0));
//	      apps_traceserver.Stop (Seconds (120.0));
//#endif


//	    std::cout << route1 ->GetSystemId() << std::endl;
//	    std::cout << route1 ->GetNDevices() << std::endl;

//	    Ptr< NetDevice > d = route1 ->GetDevice(1);

//	    std::cout << d->GetAddress() << std::endl;
//        Ptr<Icmpv6L4Protocol> icmpv6l4 = route1->GetObject<Icmpv6L4Protocol> ();
//
//        std::cout << icmpv6l4 ->GetReachableTime() << std::endl;
//
//
//        NS_LOG_INFO ("Create sockets.");
//        //Receiver socket on n1
//        TypeId tid = TypeId::LookupByName ("ns3::Ipv6RawSocketFactory");
//        Ptr<Socket> recvSink = Socket::CreateSocket (dst, tid);
//        Ipv6Address local = Ipv6Address::GetAny ();
//        recvSink->Bind (local);
//        recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
////
////
//        PacketSinkHelper sink("ns3::Ipv6RawSocketFactory",
//        		Ipv6Address::GetAny ());
//        ApplicationContainer sinkApps=sink.Install(nodes.Get(1));
//        sinkApps.Start(Seconds(0.0));
//        sinkApps.Stop(Seconds(100));
//        Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx",
//                           MakeCallback (&recvPackets));

        RipNgHelper routingHelper;
//        Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route1, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route2, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route3, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route4, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route5, routingStream);



		Ptr<FlowMonitor> flowMonitor;
		FlowMonitorHelper flowHelper;
		flowMonitor = flowHelper.Install(nodes);

		AsciiTraceHelper ascii;
		csma.EnableAsciiAll (ascii.CreateFileStream ("DEMO.tr"));
		csma.EnablePcapAll ("DEMO", true);

		// RipNgHelper routingHelper;
		// Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
		// routingHelper.PrintRoutingTableAt (Seconds (30.0), src, routingStream);


		/* Now, do the actual simulation. */

//		Simulator::Stop(Seconds());

		Simulator::Stop (Seconds (120));
//		flowMonitor = flowHelper.Install(nodes);

//		flowMonitor->SetAttribute("JitterBinWidth", ns3::DoubleValue(0.001));
//		flowMonitor->SetAttribute("DelayBinWidth", ns3::DoubleValue(0.001));
//		flowMonitor->SetAttribute("PacketSizeBinWidth", ns3::DoubleValue(20));

//		flowMonitor->SerializeToXmlFile("flowmoniter.xml", true, true);
		Simulator::Run ();
		flowMonitor->SerializeToXmlFile("new123.xml", true, true);
		Simulator::Destroy ();

		  Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApps.Get (0));
		  std::cout << "Total Bytes Received: " << sink1->GetTotalRx () << std::endl;
}



