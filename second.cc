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
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Default Network Topology
//
//
// n0---n1---n2....n3...n4...n5---n6---n7
//
//
//


using namespace ns3;
//using namespace std;

NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  //uint32_t nCsma = 4;

  CommandLine cmd;
  //cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  //nCsma = nCsma == 0 ? 1 : nCsma;
  
  

  NodeContainer nodes, p2p1, p2p2, p2p3, p2p4;
  nodes.Create (8);
  
  p2p1.Add(nodes.Get(0));
  p2p1.Add(nodes.Get(1));
  p2p2.Add(nodes.Get(1));
  p2p2.Add(nodes.Get(2));

  //std::cout<<"Error in client";
  NodeContainer csmaNodes;
  csmaNodes.Add (nodes.Get (2));
  csmaNodes.Add (nodes.Get (3));
  csmaNodes.Add (nodes.Get (4));
  csmaNodes.Add (nodes.Get (5));

  p2p3.Add(nodes.Get(5));      
  p2p3.Add(nodes.Get(6));
  p2p4.Add(nodes.Get(6));
  p2p4.Add(nodes.Get(7));
  
  
        
  PointToPointHelper pointToPoint;
  NetDeviceContainer p2pd1, p2pd2, p2pd3, p2pd4;
  
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
  
  //printf( "Error in client!\n");
  

  //NetDeviceContainer p2pdl, p2pdr;
  p2pd1 = pointToPoint.Install (p2p1);
  p2pd2 = pointToPoint.Install (p2p2);
  p2pd3 = pointToPoint.Install (p2p3);
  p2pd4 = pointToPoint.Install (p2p4);
  
  printf( "Error in client!\n");

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  InternetStackHelper stack;
  stack.Install (nodes);
  
  

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pi1;
  p2pi1 = address.Assign (p2pd1);
  
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pi2;
  p2pi2 = address.Assign (p2pd2);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);
  
  address.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pi3;
  p2pi3 = address.Assign (p2pd3);
  
  address.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pi4;
  p2pi4 = address.Assign (p2pd4);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get(7));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  
  printf( "Error in client!\n");

  UdpEchoClientHelper echoClient (p2pi4.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  //pointToPoint.EnablePcapAll ("second");
  //csma.EnablePcap ("second", csmaDevices.Get (1), true);

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
