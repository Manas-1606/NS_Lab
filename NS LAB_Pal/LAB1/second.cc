/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"

// Default Network Topology
//
//       10.1.1.0          10.1.2.0
// n0 -------------- n2 -------------n1
//                    |
//                    | 10.1.3.0
//                   n3
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (4);
  NodeContainer n0n2 = NodeContainer (nodes.Get (0), nodes.Get(2));
  NodeContainer n1n2 = NodeContainer (nodes.Get (1), nodes.Get(2));
  NodeContainer n2n3 = NodeContainer (nodes.Get (2), nodes.Get(3));
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  pointToPoint.SetQueue ("ns3::DropTailQueue");
  NetDeviceContainer d0d1 = pointToPoint.Install (n0n2);

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("8Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("3ms"));
  pointToPoint.SetQueue ("ns3::DropTailQueue");
  NetDeviceContainer d1d2 = pointToPoint.Install (n1n2);

  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("4ms"));
  pointToPoint.SetQueue ("ns3::DropTailQueue", "MaxSize", StringValue("3p"));
  NetDeviceContainer d2d3 = pointToPoint.Install (n2n3);

  InternetStackHelper stack;
  stack.Install (nodes);

  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i0i2 = ipv4.Assign (d0d1);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (d1d2);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer i2i3 = ipv4.Assign (d2d3);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (9);
  UdpEchoServerHelper echoServer_1(10);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (i0i2.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (16));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApp_1 = echoClient.Install (nodes.Get (0));
  clientApp_1.Start (Seconds (2.0));
  clientApp_1.Stop (Seconds (10.0));

  ApplicationContainer clientApp_2 = echoClient.Install (nodes.Get (1));
  clientApp_2.Start (Seconds (2.0));
  clientApp_2.Stop (Seconds (10.0));

  ApplicationContainer serverApp_1 = echoServer_1.Install (nodes.Get (3));
  serverApp_1.Start (Seconds (1.0));
  serverApp_1.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient_2 (i2i3.GetAddress (1), 10);
  echoClient_2.SetAttribute ("MaxPackets", UintegerValue (16));
  echoClient_2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient_2.SetAttribute ("PacketSize", UintegerValue (1024));  

  ApplicationContainer clientApp_3 = echoClient_2.Install (nodes.Get (2));
  clientApp_3.Start (Seconds (2.0));
  clientApp_3.Stop (Seconds (10.0));

  AnimationInterface anim ("l1_q2.xml");
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
