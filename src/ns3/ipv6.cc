#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv6-static-routing-helper.h"
#include "ns3/ipv6-routing-table-entry.h"


using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("ipv6");

static int count = 0;


void ReceivePacket (Ptr<Socket> socket)
{
	count += 1;
  Ptr<Packet> packet;
  Address from;

  while ((packet = socket->RecvFrom (from)))
    {

      if (packet->GetSize () > 0)
        {

    	  Inet6SocketAddress iaddr = Inet6SocketAddress::ConvertFrom (from);

    	    std::ostringstream oss;
    	    oss << "Received one packet! Socket: " << iaddr.GetIpv6 ()
    	        << " port: " << iaddr.GetPort ()
    	        << " at time = " << Simulator::Now ().GetSeconds ()
    	        << "";

    	  std::string x = oss.str();

    	   std::cout << x << std::endl;
         // NS_LOG_UNCOND (x);
        }
    }
}
// 接受包函数
void
AddListener(Ptr<Node> node, uint16_t port)
{
    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
    Ptr<Socket> recvSink = Socket::CreateSocket (node, tid);
    Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (), port);
    recvSink->Bind(local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));

}


int main (int argc, char **argv)
{
	GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

#if 1 // parameter
	Time topology_total = Seconds (100.0);
	Time tcp_normal_start = Seconds (60.0);
	Time tcp_normal_end  = Seconds (100.0);

#endif

#if 1 // build the network topology
	Ptr<Node> pc1_server = CreateObject<Node> ();
	Names::Add ("pc1_server", pc1_server);
	Ptr<Node> pc2_attack = CreateObject<Node> ();
	Names::Add ("pc2_attack", pc2_attack);
	Ptr<Node> pc3_normal= CreateObject<Node> ();
	Names::Add ("pc3_normal", pc3_normal);
	Ptr<Node> pc4_normal= CreateObject<Node> ();
	Names::Add ("pc4_normal", pc4_normal);
	Ptr<Node> pc5_normal= CreateObject<Node> ();
	Names::Add ("pc5_normal", pc5_normal);
	Ptr<Node> pc6_normal= CreateObject<Node> ();
	Names::Add ("pc6_normal", pc6_normal);
	Ptr<Node> pc7_normal= CreateObject<Node> ();
	Names::Add ("pc7_normal", pc7_normal);
	Ptr<Node> pc8_normal= CreateObject<Node> ();
	Names::Add ("pc8_normal", pc8_normal);

	Ptr<Node> r1 = CreateObject<Node> ();
	Names::Add ("r1", r1);
	Ptr<Node> r2 = CreateObject<Node> ();
	Names::Add ("r2", r2);
	Ptr<Node> r3 = CreateObject<Node> ();
	Names::Add ("r3", r3);
	Ptr<Node> r4 = CreateObject<Node> ();
	Names::Add ("r4", r4);
	Ptr<Node> r5 = CreateObject<Node> ();
	Names::Add ("r5", r5);
	Ptr<Node> r6 = CreateObject<Node> ();
	Names::Add ("r6", r6);
	Ptr<Node> r7 = CreateObject<Node> ();
	Names::Add ("r7", r7);
	Ptr<Node> r8 = CreateObject<Node> ();
	Names::Add ("r8", r8);
	Ptr<Node> r9 = CreateObject<Node> ();
	Names::Add ("r9", r9);
	Ptr<Node> r10 = CreateObject<Node> ();
	Names::Add ("r10", r10);
	Ptr<Node> r11= CreateObject<Node> ();
	Names::Add ("r11", r11);

	NodeContainer p1r4 (pc1_server, r4);
	NodeContainer p2r11 (pc2_attack, r11);
	NodeContainer p3r11 (pc3_normal, r11);
	NodeContainer p4r11 (pc4_normal, r11);
	NodeContainer p5r11 (pc5_normal, r11);
	NodeContainer p6r7 (pc6_normal, r7);
	NodeContainer p7r7 (pc7_normal, r7);
	NodeContainer p8r7 (pc8_normal, r7);

	NodeContainer r4r1 (r4, r1);
	NodeContainer r2r1 (r2, r1);
	NodeContainer r2r3 (r2, r3);
	NodeContainer r4r3 (r4, r3);
	NodeContainer r5r2 (r5, r2);
	NodeContainer r5r4 (r5, r4);
	NodeContainer r7r6 (r7, r6);
	NodeContainer r8r6 (r8, r6);
	NodeContainer r9r6 (r9, r6);
	NodeContainer r10r9 (r10, r9);
	NodeContainer r10r8 (r10, r8);
	NodeContainer r11r10 (r11, r10);

	NodeContainer r6r5 (r6, r5);
//以上，创建网络拓扑

	CsmaHelper csma;
	csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
	NetDeviceContainer Nr4r1 = csma.Install (r4r1);
	NetDeviceContainer Nr2r1 = csma.Install (r2r1);
	NetDeviceContainer Nr2r3 = csma.Install (r2r3);
	NetDeviceContainer Nr4r3 = csma.Install (r4r3);
	NetDeviceContainer Nr5r2 = csma.Install (r5r2);
	NetDeviceContainer Nr5r4 = csma.Install (r5r4);
	NetDeviceContainer Nr7r6 = csma.Install (r7r6);
	NetDeviceContainer Nr8r6 = csma.Install (r8r6);
	NetDeviceContainer Nr9r6 = csma.Install (r9r6);
	NetDeviceContainer Nr10r9 = csma.Install (r10r9);
	NetDeviceContainer Nr10r8 = csma.Install (r10r8);
	NetDeviceContainer Nr11r10 = csma.Install (r11r10);
	NetDeviceContainer Nr6r5 = csma.Install (r6r5);

	csma.SetChannelAttribute ("DataRate", DataRateValue (5000));
	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));

	NetDeviceContainer Np1r4 = csma.Install (p1r4);
	NetDeviceContainer Np2r11 = csma.Install (p2r11);
	NetDeviceContainer Np3r11 = csma.Install (p3r11);
	NetDeviceContainer Np4r11 = csma.Install (p4r11);
	NetDeviceContainer Np5r11 = csma.Install (p5r11);
	NetDeviceContainer Np6r7 = csma.Install (p6r7);
	NetDeviceContainer Np7r7 = csma.Install (p7r7);
	NetDeviceContainer Np8r7 = csma.Install (p8r7);

	RipNgHelper ripNgRouting;

    ripNgRouting.ExcludeInterface (r4, 1);


    ripNgRouting.ExcludeInterface(r7, 1);
    ripNgRouting.ExcludeInterface(r7, 2);
   	ripNgRouting.ExcludeInterface(r7, 3);

    ripNgRouting.ExcludeInterface(r11, 1);
    ripNgRouting.ExcludeInterface(r11, 2);
	ripNgRouting.ExcludeInterface(r11, 3);
	ripNgRouting.ExcludeInterface(r11, 4);

	Ipv6ListRoutingHelper listRH;
	listRH.Add (ripNgRouting, 0);
	Ipv6StaticRoutingHelper staticRh;
	listRH.Add (staticRh, 5);

	InternetStackHelper internetv6Nodes;
	internetv6Nodes.SetIpv4StackInstall (false);
	internetv6Nodes.Install (pc1_server);
	internetv6Nodes.Install (pc2_attack);
	internetv6Nodes.Install (pc3_normal);
	internetv6Nodes.Install (pc4_normal);
	internetv6Nodes.Install (pc5_normal);
	internetv6Nodes.Install (pc6_normal);
	internetv6Nodes.Install (pc7_normal);
	internetv6Nodes.Install (pc8_normal);

	InternetStackHelper internetv6;
	internetv6.SetIpv4StackInstall (false);
	internetv6.SetRoutingHelper (listRH);
	internetv6.Install (r1);
	internetv6.Install (r2);
	internetv6.Install (r3);
	internetv6.Install (r4);
	internetv6.Install (r5);
	internetv6.Install (r6);
	internetv6.Install (r7);
	internetv6.Install (r8);
	internetv6.Install (r9);
	internetv6.Install (r10);
	internetv6.Install (r11);
//以上，安装互联网通讯协议
	Ipv6AddressHelper ipv6_address;
	ipv6_address.SetBase (Ipv6Address ("2001:1:1::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp2r11 = ipv6_address.Assign (Np2r11);
	IICp2r11.SetForwarding (1, true);
	IICp2r11.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2001:1:2::"), Ipv6Prefix (64));
 	Ipv6InterfaceContainer IICp3r11 = ipv6_address.Assign (Np3r11);
	IICp3r11.SetForwarding (1, true);
	IICp3r11.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2001:1:3::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp4r11 = ipv6_address.Assign (Np4r11);
	IICp4r11.SetForwarding (1, true);
	IICp4r11.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2001:1:4::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp5r11 = ipv6_address.Assign (Np5r11);
	IICp5r11.SetForwarding (1, true);
	IICp5r11.SetDefaultRouteInAllNodes (1);



	ipv6_address.SetBase (Ipv6Address ("2001:8:1::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp6r7 = ipv6_address.Assign (Np6r7);
	IICp6r7.SetForwarding (1, true);
	IICp6r7.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2001:8:2::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp7r7 = ipv6_address.Assign (Np7r7);
	IICp7r7.SetForwarding (1, true);
	IICp7r7.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2001:8:3::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp8r7 = ipv6_address.Assign (Np8r7);
	IICp8r7.SetForwarding (1, true);
	IICp8r7.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2002:8:1::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICp1r4 = ipv6_address.Assign (Np1r4);
	IICp1r4.SetForwarding (1, true);
	IICp1r4.SetDefaultRouteInAllNodes (1);

	ipv6_address.SetBase (Ipv6Address ("2001:2::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr11r10 = ipv6_address.Assign (Nr11r10);
	IICr11r10.SetForwarding (0, true);
	IICr11r10.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2001:4::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr10r8 = ipv6_address.Assign (Nr10r8);
	IICr10r8.SetForwarding (0, true);
	IICr10r8.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2001:3::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr10r9 = ipv6_address.Assign (Nr10r9);
	IICr10r9.SetForwarding (0, true);
	IICr10r9.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2001:5::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr8r6 = ipv6_address.Assign (Nr8r6);
	IICr8r6.SetForwarding (0, true);
	IICr8r6.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2001:6::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr9r6 = ipv6_address.Assign (Nr9r6);
	IICr9r6.SetForwarding (0, true);
	IICr9r6.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2001:7::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr7r6 = ipv6_address.Assign (Nr7r6);
	IICr7r6.SetForwarding (0, true);
	IICr7r6.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:1::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr6r5 = ipv6_address.Assign (Nr6r5);
	IICr6r5.SetForwarding (0, true);
	IICr6r5.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:2::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr5r2 = ipv6_address.Assign (Nr5r2);
	IICr5r2.SetForwarding (0, true);
	IICr5r2.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:3::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr5r4 = ipv6_address.Assign (Nr5r4);
	IICr5r4.SetForwarding (0, true);
	IICr5r4.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:4::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr2r3 = ipv6_address.Assign (Nr2r3);
	IICr2r3.SetForwarding (0, true);
	IICr2r3.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:5::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr4r3 = ipv6_address.Assign (Nr4r3);
	IICr4r3.SetForwarding (0, true);
	IICr4r3.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:6::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr2r1 = ipv6_address.Assign (Nr2r1);
	IICr2r1.SetForwarding (0, true);
	IICr2r1.SetForwarding (1, true);

	ipv6_address.SetBase (Ipv6Address ("2002:7::"), Ipv6Prefix (64));
	Ipv6InterfaceContainer IICr4r1 = ipv6_address.Assign (Nr4r1);
	IICr4r1.SetForwarding (0, true);
	IICr4r1.SetForwarding (1, true);
#endif
//以上，分配ip地址
#if 0 //show the route table

    RipNgHelper routingHelper;
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);
    routingHelper.PrintRoutingTableAt (Seconds (80.0), r7, routingStream);

#endif

#if 0 // ping6 helper
	Ping6Helper ping6;

	ping6.SetLocal (IICp2r11.GetAddress (0, 1));
	ping6.SetRemote (IICp1r4.GetAddress (0, 1));

	ping6.SetAttribute ("MaxPackets", UintegerValue (100));
	ping6.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	ping6.SetAttribute ("PacketSize", UintegerValue (10));

	ApplicationContainer app_ping6 = ping6.Install (pc2_attack);
	app_ping6.Start (Seconds (60.0));
	app_ping6.Stop (Seconds (100.0));
#endif

#if 0 // udp server

    uint16_t port_updserver = 55;
    UdpServerHelper server (port_updserver);
    ApplicationContainer apps_udpserver = server.Install (pc1_server);

    apps_udpserver.Start (Seconds (10.0));
    apps_udpserver.Stop (Seconds (100.0));

#endif

#if 1 // pc2_attack udp flow

	      uint16_t pc2_udp_port = 8080;
	      OnOffHelper onoff_pc2_udp ("ns3::UdpSocketFactory", Inet6SocketAddress (IICp1r4.GetAddress (0, 1), pc2_udp_port));
	      onoff_pc2_udp.SetConstantRate (DataRate ("10kbps"));
	      onoff_pc2_udp.SetAttribute ("PacketSize", UintegerValue (100));

	      // onoff_pc2_udp.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	      // onoff_pc2_udp.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=2]"));

	      ApplicationContainer app_pc2_udpClient = onoff_pc2_udp.Install (pc2_attack);

	      app_pc2_udpClient.Start (Seconds (60.0));
	      app_pc2_udpClient.Stop (Seconds (100.0));

#endif

#if 0 // pc2_attack tcp flow

	      uint16_t pc2_tcp_port = 80;

	      OnOffHelper onoff_pc2_tcp ("ns3::TcpSocketFactory", Inet6SocketAddress (IICp1r4.GetAddress (0, 1), pc2_tcp_port));
//	      onoff_pc2_tcp.SetConstantRate (DataRate ("1kbps"));
//	      onoff_pc2_tcp.SetAttribute ("PacketSize", UintegerValue (100));
	      onoff_pc2_tcp.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	      onoff_pc2_tcp.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=2]"));

	      ApplicationContainer app_pc2_tcpClient = onoff_pc2_tcp.Install (pc2_attack);

	      app_pc2_tcpClient.Start (Seconds (60.0));
	      app_pc2_tcpClient.Stop (Seconds (100.0));

#endif


#if 1 //pc6_normal is a chicken to attack the server (tcp flow)
	      uint16_t pc3_tcp_port = 80;

	      OnOffHelper onoff_pc3_tcp ("ns3::TcpSocketFactory", Inet6SocketAddress (IICp1r4.GetAddress (0, 1), pc3_tcp_port));
//	      onoff_pc2_tcp.SetConstantRate (DataRate ("1kbps"));
//	      onoff_pc2_tcp.SetAttribute ("PacketSize", UintegerValue (100));
	      onoff_pc3_tcp.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	      onoff_pc3_tcp.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=2]"));

	      ApplicationContainer app_pc3_tcpClient = onoff_pc3_tcp.Install (pc3_normal);

	      app_pc3_tcpClient.Start (Seconds (60.0));
	      app_pc3_tcpClient.Stop (Seconds (100.0));
#endif


#if 1 //back ground flow with pc2-pc8

	      //udp
	      uint16_t access_port = 8080;

	      OnOffHelper onoff_udp ("ns3::UdpSocketFactory", Inet6SocketAddress (IICp1r4.GetAddress (0, 1), access_port));

	      onoff_udp.SetConstantRate (DataRate ("1kbps"));
	      onoff_udp.SetAttribute ("PacketSize", UintegerValue (100));

	      onoff_udp.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	      onoff_udp.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=2]"));

	      ApplicationContainer app_udpClient_pc3 = onoff_udp.Install (pc3_normal);
	      ApplicationContainer app_udpClient_pc4 = onoff_udp.Install (pc4_normal);
	      ApplicationContainer app_udpClient_pc5 = onoff_udp.Install (pc5_normal);
	      ApplicationContainer app_udpClient_pc6 = onoff_udp.Install (pc6_normal);
	      ApplicationContainer app_udpClient_pc7 = onoff_udp.Install (pc7_normal);
	      ApplicationContainer app_udpClient_pc8 = onoff_udp.Install (pc8_normal);

	      app_udpClient_pc3.Start (tcp_normal_start);
	      app_udpClient_pc3.Stop (tcp_normal_end);

	      app_udpClient_pc4.Start (tcp_normal_start);
	      app_udpClient_pc4.Stop (tcp_normal_end);

	      app_udpClient_pc5.Start (tcp_normal_start);
	      app_udpClient_pc5.Stop (tcp_normal_end);

	      app_udpClient_pc6.Start (tcp_normal_start);
	      app_udpClient_pc6.Stop (tcp_normal_end);

	      app_udpClient_pc7.Start (tcp_normal_start);
	      app_udpClient_pc7.Stop (tcp_normal_end);

	      app_udpClient_pc8.Start (tcp_normal_start);
	      app_udpClient_pc8.Stop (tcp_normal_end);

	      //tcp

	      uint16_t pc2_tcp_port = 80;

	      OnOffHelper onoff_pc2_tcp ("ns3::TcpSocketFactory", Inet6SocketAddress (IICp1r4.GetAddress (0, 1), pc2_tcp_port));
//	      onoff_pc2_tcp.SetConstantRate (DataRate ("1kbps"));
//	      onoff_pc2_tcp.SetAttribute ("PacketSize", UintegerValue (100));
	      onoff_pc2_tcp.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
	      onoff_pc2_tcp.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=2]"));

	      ApplicationContainer app_pc2_tcpClient = onoff_pc2_tcp.Install (pc2_attack);

	      app_pc2_tcpClient.Start (Seconds (60.0));
	      app_pc2_tcpClient.Stop (Seconds (100.0));

#endif

	      AddListener(pc1_server, 8080);


AsciiTraceHelper ascii;
csma.EnableAsciiAll (ascii.CreateFileStream ("ipv6.tr"));
csma.EnablePcapAll ("ipv6", true);


//添加可视化的代码？
Simulator::Stop (topology_total);
Simulator::Run ();
Simulator::Destroy ();
std::cout << count << std::endl;


}
