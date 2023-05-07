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
#include "ns3/csma-helper.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1 ---------n2
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

  NodeContainer p2p_nodes;
  p2p_nodes.Create (3);

  NodeContainer csma_nodes;
  csma_nodes.Add (p2p_nodes.Get (1));
  csma_nodes.Add (p2p_nodes.Get (2));
  csma_nodes.Create (3);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2p_devices_01, p2p_devices_12;
  p2p_devices_01 = p2p.Install (p2p_nodes.Get (0), p2p_nodes.Get (1));
  p2p_devices_12 = p2p.Install (p2p_nodes.Get (1), p2p_nodes.Get (2));

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue(NanoSeconds (6560)));

  NetDeviceContainer csma_devices;
  csma_devices = csma.Install (csma_nodes);

  InternetStackHelper stack;
  stack.Install (p2p_nodes.Get (0));
  stack.Install (csma_nodes);

  // Later, we add IP addresses.
  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2p_interface_1;
  p2p_interface_1 = address.Assign (p2p_devices_01);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2p_interface_2;
  p2p_interface_2 = address.Assign (p2p_devices_12);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer csma_interfaces;
  csma_interfaces = address.Assign (csma_devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer server_apps = echoServer.Install (csma_nodes.Get (4));
  server_apps.Start (Seconds (1.0));
  server_apps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (csma_interfaces.GetAddress (4), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (6));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (p2p_nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  AnimationInterface anim ("l2_q1.xml");
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
