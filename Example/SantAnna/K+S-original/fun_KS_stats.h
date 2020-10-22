/******************************************************************************

	STATISTICS EQUATIONS
	--------------------

	Equations that are not required for the model to run but may produce
	useful country- or sector-level statistics for analysis.
 
 ******************************************************************************/

/*========================= COUNTRY-LEVEL STATS ==============================*/

EQUATION( "Creal" )
/*
Real aggregated consumption
*/
RESULT( VS( GRANDPARENT, "C" ) / VS( CONSECL2, "CPI" ) )


EQUATION( "DebGDP" )
/*
Government debt on GDP ratio
*/
v[1] = VS( GRANDPARENT, "GDP" );
RESULT( v[1] > 0 ? VS( GRANDPARENT, "Deb" ) / v[1] : CURRENT )


EQUATION( "DefGDP" )
/*
Government deficit on GDP ratio
*/
v[1] = VS( GRANDPARENT, "GDP" );
RESULT( v[1] > 0 ? VS( GRANDPARENT, "Def" ) / v[1] : CURRENT )


EQUATION( "GDI" )
/*
Gross domestic income (real terms)
*/
RESULT( ( VS( CAPSECL2, "W1" ) + VS( CONSECL2, "W2" ) +
		  VS( CAPSECL2, "Pi1" ) + VS( CONSECL2, "Pi2" ) + 
		  VS( GRANDPARENT, "G" ) - VS( GRANDPARENT, "Tax" ) + 
		  VS( CAPSECL2, "PPI" ) * VS( CONSECL2, "SI" ) / VS( CONSECL2, "m2" ) ) / 
		  VS( CONSECL2, "CPI" ) )


EQUATION( "dA" )
/*
Overall labor productivity growth rate
*/
v[1] = VLS( GRANDPARENT, "A", 1 );
RESULT( v[1] > 0 ? VS( GRANDPARENT, "A" ) / v[1] - 1 : 0 )


/*========================= FINANCIAL SECTOR STATS ===========================*/

EQUATION( "BadDebAcc" )
/*
Bank accumulated losses from bad debt
*/
RESULT( CURRENT + VS( FINSECL2, "BadDeb" ) )


EQUATION( "Bda" )
/*
Firms financial fragility defined as the ratio between accumulated bad debt (loans
in default) and total bank assets (stock of loans)
*/

v[1] = max( VLS( FINSECL2, "BadDeb", 1 ), 0 );	// losses with bad debt, discarding
												// negative losses (proceedings)
v[2] = VLS( FINSECL2, "Loans", 1 );				// stock of non-defaulted loans

RESULT( ( v[1] + v[2] ) > 0 ? v[1] / ( v[1] + v[2] ) : 0 )


/*======================= CAPITAL-GOOD SECTOR STATS ==========================*/

EQUATION( "HCavg" )
/*
Number of historical clients of capital-good firms
*/
RESULT( AVES( CAPSECL2, "_HC" ) )


EQUATION( "HH1" )
/*
Normalized Herfindahl-Hirschman index for capital-good sector
*/

v[1] = i = 0;									// index accumulator & counter
CYCLES( CAPSECL2, cur, "Firm1" )
{
	v[1] += pow( VS( cur, "_f1" ), 2 );			// add the squared market shares
	++i;
}

if ( i > 1 )
	v[0] = ( v[1] - 1.0 / i ) / ( 1 - 1.0 / i );// normalize HHI
else
	v[0] = 1;
	
RESULT( v[0] )


EQUATION( "HP1" )
/*
Hymer-Pashigian index for capital-good sector
*/

v[0] = 0;										// index accumulator
CYCLES( CAPSECL2, cur, "Firm1" )
	v[0] += fabs( VLS( cur, "_f1", 1 ) - VS( cur, "_f1" ) );// sum share changes

RESULT( v[0] )	


EQUATION( "NCavg" )
/*
Number of new clients of capital-good firms
*/
RESULT( AVES( CAPSECL2, "_NC" ) )


EQUATION( "RD" )
/*
R&D expenditure of capital-good sector
*/
RESULT( SUMS( CAPSECL2, "_RD" ) )


EQUATION( "age1avg" )
/*
Average age of firms in capital-good sector
*/

v[0] = 0;										// firm age accumulator
CYCLES( CAPSECL2, cur, "Firm1" )
	v[0] += T - VS( cur, "_t1ent" ) + 1;
	
RESULT( v[0] / VS( CAPSECL2, "F1" ) )


EQUATION( "cred1c" )
/*
Total credit constraint of firms in capital-good sector
*/
RESULT( SUMS( CAPSECL2, "_cred1c" ) )


/*======================= CONSUMER-GOOD SECTOR STATS =========================*/

EQUATION( "A2sd" )
/*
Standard deviation of machine-level log labor productivity of firms in 
consumption-good sector
*/

v[1] = VS( CONSECL2, "A2" );					// average productivity
if ( v[1] <= 0 )
	END_EQUATION( 0 );							// probably no production yet

v[1] = log( v[1] + 1 );							// average log productivity

i = 0;											// valid cases count
v[0] = 0;										// square difference accumulator
CYCLES( CONSECL2, cur, "Firm2" )
{
	v[2] = VS( cur, "_A2" );
	if ( is_finite( v[2] ) && v[2] > 0 )
	{
		v[0] += pow( log( v[2] + 1 ) - v[1], 2 );
		++i;
	}
}

RESULT( i > 0 ? sqrt( v[0] / i ) : 0 )


EQUATION( "HH2" )
/*
Normalized Herfindahl-Hirschman index for consumption-good sector
*/

v[1] = j = 0;									// index accumulator & counter
CYCLES( CONSECL2, cur, "Firm2" )
{
	v[1] += pow( VS( cur, "_f2" ), 2 );			// add the squared market shares
	++j;
}

if ( j > 1 )
	v[0] = ( v[1] - 1.0 / j ) / ( 1 - 1.0 / j );// normalize HHI
else
	v[0] = 1;
	
RESULT( v[0] )


EQUATION( "HP2" )
/*
Hymer-Pashigian index for consumption-good sector
*/

v[0] = 0;										// index accumulator
CYCLES( CONSECL2, cur, "Firm2" )
	v[0] += fabs( VLS( cur, "_f2", 1 ) - VS( cur, "_f2" ) );// sum share changes

RESULT( v[0] )	


EQUATION( "L2larg" )
/*
Number of workers of largest firm in consumption-good sector
*/
RESULT( MAXS( CONSECL2, "_L2" ) )


EQUATION( "age2avg" )
/*
Average age of firms in consumption-good sector
*/

v[0] = 0;										// firm age accumulator
CYCLES( CONSECL2, cur, "Firm2" )
	v[0] += T - VS( cur, "_t2ent" ) + 1;
	
RESULT( v[0] / VS( CONSECL2, "F2" ) )


EQUATION( "cred2c" )
/*
Total credit constraint of firms in consumer-good sector
*/
RESULT( SUMS( CONSECL2, "_cred2c" ) )


EQUATION( "dN" )
/*
Change in total inventories (real terms)
*/
RESULT( VS( CONSECL2, "N" ) - VLS( CONSECL2, "N", 1 ) )


EQUATION( "mu2avg" )
/*
Weighted average mark-up of consumption-good sector
*/
RESULT( WHTAVES( CONSECL2, "_mu2", "_f2" ) )


EQUATION( "nBrochAvg" )
/*
Average number of machine brochures available to firms in consumer-good sector
*/

v[0] = 0;
CYCLES( CONSECL2, cur, "Firm2" )
	v[0] += COUNTS( cur, "Broch");

RESULT( v[0] / VS( CONSECL2, "F2" ) )


/*============================= LABOR STATS ==================================*/

EQUATION( "V" )
/*
Effective vacancy rate (unfilled positions over total labor supply)
*/
RESULT( T > 1 ? min( ( VS( CAPSECL2, "JO1" ) + VS( CONSECL2, "JO2" ) ) / 
					   VS( LABSUPL2, "Ls" ), 1 ) : 0 )


EQUATION( "dw" )
/*
Nominal average wage growth rate
*/
RESULT( log( VS( LABSUPL2, "w" ) ) - log( VLS( LABSUPL2, "w", 1 ) ) )


EQUATION( "wReal" )
/*
Centralized real wage imposed to all workers
*/
RESULT( VS( LABSUPL2, "w" )  / VS( CONSECL2, "CPI" ) )


/*============================ AGENT-LEVEL STATS =============================*/

EQUATION( "_A2e" )
/*
Machine-level effective productivity of firm in consumption-good sector
*/
i = V( "_L2" );
RESULT( i > 0 ? V( "_Q2e" ) / i : CURRENT )
