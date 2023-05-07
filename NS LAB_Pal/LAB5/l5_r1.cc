#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/packet-sink.h"
#include "ns3/netanim-module.h"
#include "ns3/csma-helper.h"

using namespace ns3;

int main()
{
	NodeContainer nodes;
	nodes.Create (5);

	CsmaHelper csma;
	csma.SetChannelAttribute ("DataRate", StringValue ("500Kbps"));
	csma.SetChannelAttribute ("Delay", StringValue("5ms"));

	NetDeviceContainer d = csma.Install(nodes);

	InternetStackHelper internet;
	internet.Install (nodes);

	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer i = ipv4.Assign (d);

	uint16_t port = 9;

	BulkSendHelper source1 (
		"ns3::TcpSocketFactory",
		InetSocketAddress (i.GetAddress (2), port));

	source1.SetAttribute ("MaxBytes", UintegerValue (0));

	ApplicationContainer sourceApp1 = source1.Install (nodes.Get (0));
	sourceApp1.Start (Seconds (0.0));
	sourceApp1.Stop (Seconds (10.0));

	BulkSendHelper source2 (
		"ns3::TcpSocketFactory",
		InetSocketAddress (i.GetAddress (3), port));

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
	csma.EnableAsciiAll (ascii.CreateFileStream ("l5_r1.tr"));

	AnimationInterface anim ("l5_r1.xml");
  	Simulator::Run ();
  	Simulator::Destroy ();


	return 0;
}