
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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("test2");



/**
 * \param socket Pointer to socket.
 *
 * Packet receiving sink.
 */
void
ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  Address from;
  uint8_t ttls;
  uint8_t nh;

  while ((packet = socket->RecvFrom (from)))
    {
	  ttls=socket->GetIpv6HopLimit();
	  nh=socket->GetIpv6Tclass();
	  std::cout<<ttls<<std::endl;
	  std::cout<<nh;
	  // std::cout << from << std::endl;
      if (packet->GetSize () > 0)
        {
    	  packet->EnablePrinting();
    	  uint8_t buffer[16];
    	 //	     	  s=packet->Serialize(buf,32);
    	 	     	  packet->CopyData(buffer,16);
    	 	    	  std::cout<<"溯源路由器地址段：";
//    	 	    	  std::cout<<ttls<<std::endl;
    	 	    	  for(int i=0;i<16;i++){
    	 	    		  printf("%02X",buffer[i]);
    	 	    	  }
    	   printf("\n");
    	  Inet6SocketAddress iaddr = Inet6SocketAddress::ConvertFrom (from);
    	    std::ostringstream oss;
    	    oss << "Received one tracesource packet! Socket: " << iaddr.GetIpv6 ()
    	        << " port: " << iaddr.GetPort ()
    	        << " at time = " << Simulator::Now ().GetSeconds ()
    	        << "";

    	  std::string x = oss.str();
    	  std::cout << x << std::endl;

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
    recvSink->Bind(local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

}

void shit(Ptr<Socket> socket){
	  Ptr<Packet> packet;
	  Address from;

	  while ((packet = socket->RecvFrom (from)))
	    {

		  std::cout << socket << std::endl;
	    }
}

void AddPackeSink(Ptr<Node> node, uint16_t port)
{

    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");

    PacketSinkHelper sink ("ns3::UdpSocketFactory",
                             Inet6SocketAddress (Ipv6Address::GetAny (), port));
      ApplicationContainer sinkApps = sink.Install (node);

      Ptr<Socket> sinkSocket = Socket::CreateSocket (node, tid);  // node 1, receiver

      sinkSocket->SetRecvCallback (MakeCallback (&shit));


      sinkApps.Start (Seconds (0.0));
      sinkApps.Stop (Seconds (100.0));
}




int main (int argc, char **argv)
{

	LogComponentEnable ("Packet", LOG_LEVEL_INFO);
//	LogComponentEnable ("Ipv6-l3-protocol", LOG_LEVEL_INFO);

        // LogComponentEnable ("Ipv6L3Protocol", LOG_LEVEL_ALL);

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


		NodeContainer net1 (src, route1);
		NodeContainer net2 (route1, route2);
		NodeContainer net3 (route2, route3);
		NodeContainer net4 (route3, route4);
		NodeContainer net5 (route4, route5);
		NodeContainer net6 (route5, route6);
		NodeContainer net7 (route6, dst);
//		NodeContainer net8 (route6, dst);
		NodeContainer routers (route1, route2, route3,route4,route5);
		NodeContainer routers1 (route6);
		NodeContainer nodes (src, dst);



		CsmaHelper csma;
		csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
		csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
		NetDeviceContainer ndc1 = csma.Install (net1);
		NetDeviceContainer ndc2 = csma.Install (net2);
		NetDeviceContainer ndc3 = csma.Install (net3);
		NetDeviceContainer ndc7 = csma.Install (net7);
//		NetDeviceContainer ndc8 = csma.Install (net8);
		CsmaHelper csma1;
		csma1.SetChannelAttribute ("DataRate", DataRateValue (5000000));
		csma1.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (3)));
		NetDeviceContainer ndc4 = csma1.Install (net4);
		NetDeviceContainer ndc5 = csma1.Install (net5);
		NetDeviceContainer ndc6 = csma1.Install (net6);



		RipNgHelper ripNgRouting;
		ripNgRouting.ExcludeInterface (route1, 1);
		ripNgRouting.ExcludeInterface (route6, 2);



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

		ipv6.SetBase (Ipv6Address ("2001:5:1:"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic5 = ipv6.Assign (ndc5);
		iic5.SetForwarding (0, true);
		iic5.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:5:2:"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic6 = ipv6.Assign (ndc6);
		iic6.SetForwarding (0, true);
		iic6.SetForwarding (1, true);

		ipv6.SetBase (Ipv6Address ("2001:6::"), Ipv6Prefix (64));
		Ipv6InterfaceContainer iic7 = ipv6.Assign (ndc7);
		iic7.SetForwarding (0, true);
//		iic7.SetForwarding (1, true);
		iic7.SetDefaultRouteInAllNodes (0);

//		ipv6.SetBase (Ipv6Address ("2001:7::"), Ipv6Prefix (64));
//		Ipv6InterfaceContainer iic8 = ipv6.Assign (ndc8);
//		iic8.SetForwarding (0, true);
//		iic8.SetDefaultRouteInAllNodes (0);



#if 0  // ping6

		Ping6Helper ping6;
		ping6.SetLocal (iic1.GetAddress (0, 1));
		ping6.SetRemote (iic7.GetAddress (1, 1));

		ping6.SetAttribute ("MaxPackets", UintegerValue (100));
		ping6.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
		ping6.SetAttribute ("PacketSize", UintegerValue (20));
		ApplicationContainer app_ping6 = ping6.Install (src);
		app_ping6.Start (Seconds (20.0));
		app_ping6.Stop (Seconds (30.0));

		UdpClientHelper client (Ipv6Address ("2001:1::1"), 8000);
		client.SetAttribute ("MaxPackets", UintegerValue (320));
		client.SetAttribute ("Interval", TimeValue (Seconds (1)));
		client.SetAttribute ("PacketSize", UintegerValue (20));
		ApplicationContainer app_udpClient = client.Install (src);

		app_udpClient.Start (Seconds (2.0));
		app_udpClient.Stop (Seconds (19.0));

		UdpServerHelper server (8000);
		ApplicationContainer app_udpServer = server.Install (dst);

		app_udpServer.Start (Seconds (1.0));
		app_udpServer.Stop (Seconds (20.0));

#endif

#if 0 // the address of router

	      Ptr<NetDevice> device = ndc.Get(1) ;
	      Ptr<Node> node = device->GetNode ();
	      Ptr<Ipv6> i6 = node->GetObject<Ipv6> ();
	      // i6 ->GetAddress(1,1);
	      int32_t interface = i6->GetInterfaceForDevice (device);
	      std :: cout << interface ;
#endif

#if 1// onoff application

	      uint16_t port = 10;   // Discard port (RFC 863)

	      OnOffHelper onoff ("ns3::UdpSocketFactory",
	                         Inet6SocketAddress (iic7.GetAddress (1, 1), port));

	      onoff.SetConstantRate (DataRate ("1kbps"));
	      onoff.SetAttribute ("PacketSize", UintegerValue (100));

	      ApplicationContainer apps = onoff.Install (src);

	      apps.Start (Seconds (31.0));
	      apps.Stop (Seconds (40.0));




	      // Ipv4GlobalRoutingHelper g;
	      // Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("dynamic-global-routing.routes", std::ios::out);
	      // g.PrintRoutingTableAllAt (Seconds (12), routingStream);
#endif

#if 0 // udp server
	      uint16_t port_updserver = 55;
	      UdpServerHelper server (port_updserver);
	      ApplicationContainer apps_udpserver = server.Install (dst);
	      apps_udpserver.Start (Seconds (30.0));
	      apps_udpserver.Stop (Seconds (41.0));
#endif

#if 0 // trace server
	      uint16_t traceport = 3080;  // well-known echo port number
	      UdpServerHelper traceserver (traceport);
	      ApplicationContainer apps_traceserver = traceserver.Install (dst);
	      apps_traceserver.Start (Seconds (0.0));
	      apps_traceserver.Stop (Seconds (120.0));
#endif


	    AddListener(dst,33);
	    // AddPackeSink(dst, 33);
	    std::cout << route1 ->GetSystemId() << std::endl;
	    std::cout << route1 ->GetNDevices() << std::endl;

	    Ptr< NetDevice > d = route1 ->GetDevice(1);

	    std::cout << d->GetAddress() << std::endl;
        Ptr<Icmpv6L4Protocol> icmpv6l4 = route1->GetObject<Icmpv6L4Protocol> ();

        std::cout << icmpv6l4 ->GetReachableTime() << std::endl;



        RipNgHelper routingHelper;
//        Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route1, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route2, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route3, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route4, routingStream);
//        routingHelper.PrintRoutingTableAt (Seconds (30.0), route5, routingStream);





		AsciiTraceHelper ascii;
		csma.EnableAsciiAll (ascii.CreateFileStream ("DEMO.tr"));
		csma.EnablePcapAll ("DEMO", true);

		// RipNgHelper routingHelper;
		// Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
		// routingHelper.PrintRoutingTableAt (Seconds (30.0), src, routingStream);


		/* Now, do the actual simulation. */
		Simulator::Stop (Seconds (120));
		Simulator::Run ();
		Simulator::Destroy ();

}



