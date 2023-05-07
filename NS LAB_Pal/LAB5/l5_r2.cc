#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main()
{
	NodeContainer nodes;
	nodes.Create (10);

	NodeContainer n0n1 = NodeContainer (nodes.Get (0), nodes.Get (1));
	NodeContainer n1n2 = NodeContainer (nodes.Get (1), nodes.Get (2));

	NodeContainer n0n3 = NodeContainer (nodes.Get (0), nodes.Get (3));
	NodeContainer n1n4 = NodeContainer (nodes.Get (1), nodes.Get (4));
	NodeContainer n2n5 = NodeContainer (nodes.Get (2), nodes.Get (5));

	NodeContainer n3n4 = NodeContainer (nodes.Get (3), nodes.Get (4));
	NodeContainer n4n5 = NodeContainer (nodes.Get (4), nodes.Get (5));

	NodeContainer n3n6 = NodeContainer (nodes.Get (3), nodes.Get (6));
	NodeContainer n4n7 = NodeContainer (nodes.Get (4), nodes.Get (7));
	NodeContainer n5n8 = NodeContainer (nodes.Get (5), nodes.Get (8));

	NodeContainer n6n7 = NodeContainer (nodes.Get (6), nodes.Get (7));
	NodeContainer n7n8 = NodeContainer (nodes.Get (7), nodes.Get (8));

	NodeContainer n6n9 = NodeContainer (nodes.Get (6), nodes.Get (9));
	NodeContainer n7n9 = NodeContainer (nodes.Get (7), nodes.Get (9));
	NodeContainer n8n9 = NodeContainer (nodes.Get (8), nodes.Get (9));


	PointToPointHelper p2p;
	p2p.SetDeviceAttribute ("DataRate", StringValue ("500Kbps"));
	p2p.SetChannelAttribute ("Delay", StringValue("5ms"));

	NetDeviceContainer d0d1 = p2p.Install (n0n1);
	NetDeviceContainer d1d2 = p2p.Install (n1n2);

	NetDeviceContainer d0d3 = p2p.Install (n0n3);
	NetDeviceContainer d1d4 = p2p.Install (n1n4);
	NetDeviceContainer d2d5 = p2p.Install (n2n5);

	NetDeviceContainer d3d4 = p2p.Install (n3n4);
	NetDeviceContainer d4d5 = p2p.Install (n4n5);

	NetDeviceContainer d3d6 = p2p.Install (n3n6);
	NetDeviceContainer d4d7 = p2p.Install (n4n7);
	NetDeviceContainer d5d8 = p2p.Install (n5n8);

	NetDeviceContainer d6d7 = p2p.Install (n6n7);
	NetDeviceContainer d7d8 = p2p.Install (n7n8);

	NetDeviceContainer d6d9 = p2p.Install (n6n9);
	NetDeviceContainer d7d9 = p2p.Install (n7n9);
	NetDeviceContainer d8d9 = p2p.Install (n8n9);

	InternetStackHelper internet;
	internet.Install (nodes);

	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
	ipv4.SetBase ("10.1.2.0", "255.255.255.0");
	Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
	ipv4.SetBase ("10.1.3.0", "255.255.255.0");
	Ipv4InterfaceContainer i0i3 = ipv4.Assign (d0d3);
	ipv4.SetBase ("10.1.4.0", "255.255.255.0");
	Ipv4InterfaceContainer i1i4 = ipv4.Assign (d1d4);
	ipv4.SetBase ("10.1.5.0", "255.255.255.0");
	Ipv4InterfaceContainer i2i5 = ipv4.Assign (d2d5);
	ipv4.SetBase ("10.1.6.0", "255.255.255.0");
	Ipv4InterfaceContainer i3i4 = ipv4.Assign (d3d4);
	ipv4.SetBase ("10.1.7.0", "255.255.255.0");
	Ipv4InterfaceContainer i4i5 = ipv4.Assign (d4d5);
	ipv4.SetBase ("10.1.8.0", "255.255.255.0");
	Ipv4InterfaceContainer i3i6 = ipv4.Assign (d3d6);
	ipv4.SetBase ("10.1.9.0", "255.255.255.0");
	Ipv4InterfaceContainer i4i7 = ipv4.Assign (d4d7);
	ipv4.SetBase ("10.1.10.0", "255.255.255.0");
	Ipv4InterfaceContainer i5i8 = ipv4.Assign (d5d8);
	ipv4.SetBase ("10.1.11.0", "255.255.255.0");
	Ipv4InterfaceContainer i6i7 = ipv4.Assign (d6d7);
	ipv4.SetBase ("10.1.12.0", "255.255.255.0");
	Ipv4InterfaceContainer i7i8 = ipv4.Assign (d7d8);
	ipv4.SetBase ("10.1.13.0", "255.255.255.0");
	Ipv4InterfaceContainer i6i9 = ipv4.Assign (d6d9);
	ipv4.SetBase ("10.1.14.0", "255.255.255.0");
	Ipv4InterfaceContainer i7i9 = ipv4.Assign (d7d9);
	ipv4.SetBase ("10.1.15.0", "255.255.255.0");
	Ipv4InterfaceContainer i8i9 = ipv4.Assign (d8d9);


	uint16_t port = 9;

	UdpEchoServerHelper server(port);
	ApplicationContainer server_app = server.Install(nodes.Get (0));
	server_app.Start (Seconds(0.0));
	server_app.Stop (Seconds (10.0));

	uint32_t packet_size = 1024;
	uint32_t max_packet_count = 6;
	Time inter_packet_interval = Seconds (0.1);

	UdpEchoClientHelper client (i0i1.GetAddress (0), port);
	client.SetAttribute ("MaxPackets", UintegerValue (max_packet_count));
	client.SetAttribute ("Interval", TimeValue (inter_packet_interval));
	client.SetAttribute ("PacketSize", UintegerValue (packet_size));
	ApplicationContainer client_apps = client.Install (NodeContainer(nodes.Get (5), nodes.Get (9)));
	client_apps.Start (Seconds (0.0));
	client_apps.Stop (Seconds (10.0));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	AsciiTraceHelper ascii;
	p2p.EnableAsciiAll (ascii.CreateFileStream ("l5_r2.tr"));

	AnimationInterface anim ("l5_r2.xml");
  	Simulator::Run ();
  	Simulator::Destroy ();


	return 0;
}