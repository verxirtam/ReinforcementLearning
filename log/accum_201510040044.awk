#!/usr/bin/awk -f
{
	N++;
	CostMean+=$3;
	CostSq += $3*$3;
	CountMean += $4;
	CountSq += $4*$4;
}
END{
	#print N;

	CostMean/=N;
	CostUV=CostSq/(N-1)-CostMean*CostMean*N/(N-1);
	CostIntWidth2=1.9842*sqrt(CostUV)/sqrt(N);
	CostIntMin=CostMean-CostIntWidth2;
	CostIntMax=CostMean+CostIntWidth2;
	#print CostMean" "CostUV" "sqrt(CostUV)" ["CostIntMin", "CostIntMax"]";

	CountMean/=N;
	CountUV=CountSq/(N-1)-CountMean*CountMean*N/(N-1);
	CountIntWidth2=1.9842*sqrt(CountUV)/sqrt(N);
	CountIntMin=CountMean-CountIntWidth2;
	CountIntMax=CountMean+CountIntWidth2;
	#print CountMean" "CountUV" "sqrt(CountUV)" ["CountIntMin", "CountIntMax"]";

	#print "["CostIntMin/CountIntMax", "CostIntMax/CountIntMin"]";
	print CostIntMin/CountIntMax"\t"CostIntMax/CountIntMin;
} 

