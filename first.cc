#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

//definicao de namespace
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("PrimeiroScriptExemplo");

int
main(int argc, char *argv[])
{
	//pegando logs
	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

	//determinando numero de nos
	NodeContainer nodes;
	nodes.Create (2);

	//Tipo de tecnologia a ser utilizada na comunicacao
	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

	//Instalacao da tecnologia nos nodes
	NetDeviceContainer devices;
	devices = pointToPoint.Install (nodes);

	//Atribuir protocolos aos nodes
	InternetStackHelper stack;
	stack.Install (nodes);

	//atribuicao de endereco ip aos nos
	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer interfaces = address.Assign (devices);
	//cria um servidor respondendo via udp na porta 9
	UdpEchoServerHelper echoServer (9);

	//instala o servidor em um node e inicia e termina o servidor
	ApplicationContainer serverApps = echoServer.Install(nodes.Get (1));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (10.0));

	//criacao de um cliente juntamente com seus parametros
	UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
	echoClient.SetAttribute ("PacketSize",UintegerValue (1024));

	//Instala o client e inicia e termina
	ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (10.0));

	//roda simulacao
	Simulator::Run ();
	Simulator::Destroy();
	return 0;
}
