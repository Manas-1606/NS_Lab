BEGIN {
packet_id=0;
packet[packet_id]=0;
Packet_count = 0;
Aggergate_delay=0;
Aggergate_Throughput=0;
count1=0;
Average_delay=0;
Average_Throughput=0;
Packet_Throughput=0;
count=0;
Packet_drop_count=0;
Packet_reached_count=0;
pdf=0;
start_time[packet_id]=0;
packet_count=0;
}

{
   action = $1;
   time = $2;
   type = $21;
   pktsize = $28;
   packet_id = $19;


    if ( packet[packet_id] == 0 )
      {
        packet[packet_id] = 1;
        size[packet_id]=$28;
        Packet_count=Packet_count+1;
      }

    if (action == "d" )
      {
        Packet_drop_count=Packet_drop_count+1;
      }

    if ( start_time[packet_id] == 0 ) 
        start_time[packet_id] = time;
        printf("start_time: %f \n", time);

    if (  action != "d" )
     {

      if ( action == "r" )
         {
         end_time[packet_id] = time;
         printf("end_time: %f \n", time);
         }
     }

}
END {

    for ( packet_id = 0; packet_id <= Packet_count; packet_id++ )

    {
       start = start_time[packet_id];
       printf("start_time: %f \n", start);
       end = end_time[packet_id];
       printf("end_time: %f \n", end);
       delay = end - start;
       printf("delay: %f\n", delay);

       if(delay >= 0.000000)
       {
         Aggergate_delay=Aggergate_delay+delay;
         Packet_Throughput=(size[packet_id]*8)/delay;
         Aggergate_Throughput=Aggergate_Throughput+Packet_Throughput;
         count=count+1;
       }
   }
 #    printf("%f \n", count);
     Average_delay=(Aggergate_delay/count);
     Average_Throughput=(Aggergate_Throughput/count);
     printf("Average_delay=%f Seconds \n", Average_delay);
     printf("\nAverage_Throughput=%f bits/second \n\n",Average_Throughput);
     Packet_reached_count=Packet_count - Packet_drop_count;
     pdf=(Packet_reached_count/Packet_count)*100;

     printf("\nNumber of Packet Sent=%f  \n",Packet_count);
     printf("Number of Packet Dropped=%f  \n",Packet_drop_count);
     printf("Number of Packet Received=%f  \n",Packet_reached_count);
     printf("Packet Delivery Fraction (PDF)=%f percent  \n\n",pdf); 

 
	
}



