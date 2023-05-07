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
	nodes.Create (4);

	NodeContainer n0n1 = NodeContainer (nodes.Get (0), nodes.Get (1));
	NodeContainer n1n2 = NodeContainer (nodes.Get (1), nodes.Get (2));
	NodeContainer n0n3 = NodeContainer (nodes.Get (0), nodes.Get (3));

	PointToPointHelper p2p;
	p2p.SetDeviceAttribute ("DataRate", StringValue ("500Kbps"));
	p2p.SetChannelAttribute ("Delay", StringValue("5ms"));

	NetDeviceContainer d0d1 = p2p.Install(n0n1);
	NetDeviceContainer d1d2 = p2p.Install(n1n2);
	NetDeviceContainer d0d3 = p2p.Install(n0n3);

	InternetStackHelper internet;
	internet.Install (nodes);

	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer i0i1 = ipv4.Assign (d0d1);
	ipv4.SetBase ("10.1.2.0", "255.255.255.0");
	Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);
	ipv4.SetBase ("10.1.3.0", "255.255.255.0");
	Ipv4InterfaceContainer i0i3 = ipv4.Assign (d0d3);

	uint16_t port = 9;

	BulkSendHelper source1 (
		"ns3::TcpSocketFactory",
		InetSocketAddress (i0i3.GetAddress (1), port));

	source1.SetAttribute ("MaxBytes", UintegerValue (0));

	ApplicationContainer sourceApp1 = source1.Install (nodes.Get (0));
	sourceApp1.Start (Seconds (0.0));
	sourceApp1.Stop (Seconds (10.0));

	BulkSendHelper source2 (
		"ns3::TcpSocketFactory",
		InetSocketAddress (i1i2.GetAddress (1), port));

	source2.SetAttribute ("MaxBytes", UintegerValue (0));

	ApplicationContainer sourceApp2 = source2.Install (nodes.Get (0));
	sourceApp2.Start (Seconds (0.0));
	sourceApp2.Stop (Seconds (10.0));

	PacketSinkHelper sink (
		"ns3::TcpSocketFactory",
		InetSocketAddress (Ipv4Address::GetAny (), port));

	ApplicationContainer sinkApps = sink.Install (NodeContainer (nodes.Get (2), nodes.Get (3)));
	sinkApps.Start (Seconds (0.0));
	sinkApps.Stop (Seconds (10.0));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	AsciiTraceHelper ascii;
	p2p.EnableAsciiAll (ascii.CreateFileStream ("l5_test_tcp.tr"));

	AnimationInterface anim ("l5_test_tcp.xml");
  	Simulator::Run ();
  	Simulator::Destroy ();


	return 0;
}