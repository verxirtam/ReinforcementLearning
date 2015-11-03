#!/usr/bin/awk -f
BEGIN{
	TDist95Point[1]=12.7062;
	TDist95Point[2]=4.3027;
	TDist95Point[3]=3.1824;
	TDist95Point[4]=2.7765;
	TDist95Point[5]=2.5706;
	TDist95Point[6]=2.4469;
	TDist95Point[7]=2.3646;
	TDist95Point[8]=2.3060;
	TDist95Point[9]=2.2622;
	TDist95Point[10]=2.2281;
	TDist95Point[11]=2.2010;
	TDist95Point[12]=2.1788;
	TDist95Point[13]=2.1604;
	TDist95Point[14]=2.1448;
	TDist95Point[15]=2.1315;
	TDist95Point[16]=2.1199;
	TDist95Point[17]=2.1098;
	TDist95Point[18]=2.1009;
	TDist95Point[19]=2.0930;
	TDist95Point[20]=2.0860;
	TDist95Point[21]=2.0796;
	TDist95Point[22]=2.0739;
	TDist95Point[23]=2.0687;
	TDist95Point[24]=2.0639;
	TDist95Point[25]=2.0595;
	TDist95Point[26]=2.0555;
	TDist95Point[27]=2.0518;
	TDist95Point[28]=2.0484;
	TDist95Point[29]=2.0452;
	TDist95Point[30]=2.0423;
	TDist95Point[31]=2.0395;
	TDist95Point[32]=2.0369;
	TDist95Point[33]=2.0345;
	TDist95Point[34]=2.0322;
	TDist95Point[35]=2.0301;
	TDist95Point[36]=2.0281;
	TDist95Point[37]=2.0262;
	TDist95Point[38]=2.0244;
	TDist95Point[39]=2.0227;
	TDist95Point[40]=2.0211;
	TDist95Point[41]=2.0195;
	TDist95Point[42]=2.0181;
	TDist95Point[43]=2.0167;
	TDist95Point[44]=2.0154;
	TDist95Point[45]=2.0141;
	TDist95Point[46]=2.0129;
	TDist95Point[47]=2.0117;
	TDist95Point[48]=2.0106;
	TDist95Point[49]=2.0096;
	TDist95Point[50]=2.0086;
	TDist95Point[51]=2.0076;
	TDist95Point[52]=2.0066;
	TDist95Point[53]=2.0057;
	TDist95Point[54]=2.0049;
	TDist95Point[55]=2.0040;
	TDist95Point[56]=2.0032;
	TDist95Point[57]=2.0025;
	TDist95Point[58]=2.0017;
	TDist95Point[59]=2.0010;
	TDist95Point[60]=2.0003;
	TDist95Point[61]=1.9996;
	TDist95Point[62]=1.9990;
	TDist95Point[63]=1.9983;
	TDist95Point[64]=1.9977;
	TDist95Point[65]=1.9971;
	TDist95Point[66]=1.9966;
	TDist95Point[67]=1.9960;
	TDist95Point[68]=1.9955;
	TDist95Point[69]=1.9949;
	TDist95Point[70]=1.9944;
	TDist95Point[71]=1.9939;
	TDist95Point[72]=1.9935;
	TDist95Point[73]=1.9930;
	TDist95Point[74]=1.9925;
	TDist95Point[75]=1.9921;
	TDist95Point[76]=1.9917;
	TDist95Point[77]=1.9913;
	TDist95Point[78]=1.9908;
	TDist95Point[79]=1.9905;
	TDist95Point[80]=1.9901;
	TDist95Point[81]=1.9897;
	TDist95Point[82]=1.9893;
	TDist95Point[83]=1.9890;
	TDist95Point[84]=1.9886;
	TDist95Point[85]=1.9883;
	TDist95Point[86]=1.9879;
	TDist95Point[87]=1.9876;
	TDist95Point[88]=1.9873;
	TDist95Point[89]=1.9870;
	TDist95Point[90]=1.9867;
	TDist95Point[91]=1.9864;
	TDist95Point[92]=1.9861;
	TDist95Point[93]=1.9858;
	TDist95Point[94]=1.9855;
	TDist95Point[95]=1.9852;
	TDist95Point[96]=1.9850;
	TDist95Point[97]=1.9847;
	TDist95Point[98]=1.9845;
	TDist95Point[99]=1.9842;
	TDist95Point[100]=1.9840;




}
{
	if($4>0){N++;}
	CostSum+=$3;
	CostSq += $3*$3;
	CountSum += $4;
	CountSq += $4*$4;
	#################################################
	#print N;

	CostMean=CostSum/N;
	CostUV=CostSq/(N-1)-CostMean*CostMean*N/(N-1);
	CostIntWidth2=TDist95Point[(N-1>100)?(100):(N-1)]*sqrt(CostUV)/sqrt(N);
	CostIntMin=CostMean-CostIntWidth2;
	CostIntMax=CostMean+CostIntWidth2;
	#print CostMean" "CostUV" "sqrt(CostUV)" ["CostIntMin", "CostIntMax"]";

	CountMean=CountSum/N;
	CountUV=CountSq/(N-1)-CountMean*CountMean*N/(N-1);
	CountIntWidth2=TDist95Point[(N-1>100)?(100):(N-1)]*sqrt(CountUV)/sqrt(N);
	CountIntMin=CountMean-CountIntWidth2;
	CountIntMax=CountMean+CountIntWidth2;
	#print CountMean" "CountUV" "sqrt(CountUV)" ["CountIntMin", "CountIntMax"]";

	#print "["CostIntMin/CountIntMax", "CostIntMax/CountIntMin"]";
	print CostIntMin/CountIntMax"\t"CostIntMax/CountIntMin;
	#################################################
}
END{
} 

