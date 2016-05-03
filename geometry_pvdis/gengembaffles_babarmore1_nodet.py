#!/usr/bin/python

frontgemdet1no = 1000
frontgemdet2no = 2000

#bafmat = "Kryptonite"
bafmat = "Lead"

####################################################################
# Beampipe/general parameters

piperad1 = 2.0
piperad2 = 20.0
outrad   = 140.0

nsector = 30

####################################################################
# GEM parameters

z1 = 157.5
z2 = 185.5

r1min = 48.0 
r1max = 122.0

r2min = 59.0
r2max = 143.0

rot1 = -2
rot2 = -4

nlayer  = 23

gemthick = (0.012,0.0003,0.012,0.005,0.0005,0.3,0.0005,0.005,0.0005,0.2,0.0005,0.005,0.0005,0.2,0.0005,0.005,0.0005,0.2,0.001,0.005,0.012,0.0003,0.012)
gemmat  = ("NEMAG10","NOMEX","NEMAG10","Kapton","GEMCopper","GEMgas","GEMCopper","Kapton","GEMCopper","GEMgas","GEMCopper","Kapton","GEMCopper","GEMgas","GEMCopper","Kapton","GEMCopper","GEMgas","GEMCopper","Kapton","NEMAG10","NOMEX","NEMAG10")

detlayer = (4,5,6,18)


####################################################################
# Baffle parameters

nbaf = 11

nblock = 20

z_baf = (40,54,68,82,96,110,124,138,152,166,180)
bafdepth = 9.0


rin_outbaf  = (34.70,44.54,54.30,64.01,73.80,83.48,93.30,102.95,112.80,122.43,132.00)
rout_outbaf = (42,52,62,72,82,92,102,112,120,132,140)


rin_inbaf  = (3.89,8.95,14.00,18.50,24.00,30.45,35.90,41.40,47.90,52.35,58.80)
rout_inbaf = (3.90,9.62,15.30,20.92,26.60,32.22,37.90,43.52,49.20,54.82,60.40)

rot_offset =(-5.6, -5.00,-4.4, -3.85,-3.3, -2.70,-2.1, -1.55,-0.9, -0.4,0.1)
 #extrapolated rotation offsets for new baffle locations. Rakitha Thu Aug 29 15:05:57 EDT 2013

####################################################################
# Baffle definition

mybaf = ( #Rin     Rout   SPhi    DPhi
#1 plate
[	 3.90 ,   5.44 ,  176.26 , 183.00 ,
	 5.44 ,   6.98 ,  176.44 , 182.13 ,
	 6.98 ,   8.52 ,  176.61 , 181.67 ,
	 8.52 ,  10.06 ,  176.75 , 181.78 ,
	10.06 ,  11.60 ,  176.89 , 181.90 ,
	11.60 ,  13.14 ,  177.03 , 182.01 ,
	13.14 ,  14.68 ,  177.18 , 182.12 ,
	14.68 ,  16.22 ,  177.32 , 182.24 ,
	16.22 ,  17.76 ,  177.46 , 182.35 ,
	17.76 ,  19.30 ,  177.60 , 182.46 ,
	19.30 ,  20.84 ,  177.74 , 182.58 ,
	20.84 ,  22.38 ,  177.88 , 182.69 ,
	22.38 ,  23.92 ,  178.02 , 182.80 ,
	23.92 ,  25.46 ,  178.16 , 182.92 ,
	25.46 ,  27.00 ,  178.30 , 183.03 ,
	27.00 ,  28.54 ,  178.44 , 183.15 ,
	28.54 ,  30.08 ,  178.58 , 183.26 ,
	30.08 ,  31.62 ,  178.72 , 183.37 ,
	31.62 ,  33.16 ,  178.86 , 183.49 ,
	33.16 ,  34.70 ,  179.00 , 183.60 	],
#plate 2 : I extrapolated for new z locations. Rakitha Thu Aug 29 15:05:57 EDT 2013
    [ 9.62,	11.36,	176.09,	182.91,
      11.36,	13.11,	176.29,	182.38,
      13.11,	14.86,	176.48,	182.11,
      14.86,	16.60,	176.67,	182.08,
      16.60,	18.35,	176.86,	182.06,
      18.35,	20.09,	177.05,	182.06,
      20.09,	21.84,	177.25,	182.11,
      21.84,	23.59,	177.43,	182.21,
      23.59,	25.33,	177.61,	182.31,
      25.33,	27.08,	177.77,	182.44,
      27.08,	28.82,	177.92,	182.57,
      28.82,	30.57,	178.06,	182.71,
      30.57,	32.32,	178.20,	182.83,
      32.32,	34.06,	178.36,	182.96,
      34.06,	35.81,	178.52,	183.09,
      35.81,	37.56,	178.68,	183.23,
      37.56,	39.30,	178.84,	183.35,
      39.30,	41.05,	179.00,	183.48,
      41.05,	42.79,	179.16,	183.61,
      42.79,	44.54,	179.32,	183.74 ],

#3 plate
[	15.30 ,  17.25 ,  175.93 , 182.78 ,
	17.25 ,  19.20 ,  176.14 , 182.56 ,
	19.20 ,  21.15 ,  176.35 , 182.36 ,
	21.15 ,  23.10 ,  176.58 , 182.19 ,
	23.10 ,  25.05 ,  176.81 , 182.05 ,
	25.05 ,  27.00 ,  177.06 , 181.93 ,
	27.00 ,  28.95 ,  177.28 , 181.98 ,
	28.95 ,  30.90 ,  177.46 , 182.12 ,
	30.90 ,  32.85 ,  177.64 , 182.26 ,
	32.85 ,  34.80 ,  177.82 , 182.41 ,
	34.80 ,  36.75 ,  178.00 , 182.55 ,
	36.75 ,  38.70 ,  178.18 , 182.70 ,
	38.70 ,  40.65 ,  178.35 , 182.84 ,
	40.65 ,  42.60 ,  178.53 , 182.98 ,
	42.60 ,  44.55 ,  178.71 , 183.13 ,
	44.55 ,  46.50 ,  178.89 , 183.27 ,
	46.50 ,  48.45 ,  179.07 , 183.41 ,
	48.45 ,  50.40 ,  179.25 , 183.56 ,
	50.40 ,  52.35 ,  179.42 , 183.70 ,
	52.35 ,  54.30 ,  179.60 , 183.84 	],
#plate 4: I extrapolated for new z locations. Rakitha Thu Aug 29 15:05:57 EDT 2013
    [ 20.92, 23.07,	175.82,	182.56,
      23.07,	25.23,	176.05,	182.20,
      25.23,	27.38,	176.28,	182.02,
      27.38,	29.54,	176.51,	182.01,
      29.54,	31.69,	176.74,	182.02,
      31.69,	33.85,	176.99,	182.04,
      33.85,	36.00,	177.23,	182.11,
      36.00,	38.15,	177.47,	182.20,
      38.15,	40.31,	177.71,	182.31,
      40.31,	42.46,	177.94,	182.45,
      42.46,	44.62,	178.16,	182.60,
      44.62,	46.77,	178.38,	182.76,
      46.77,	48.93,	178.57,	182.91,
      48.93,	51.08,	178.77,	183.06,
      51.08,	53.24,	178.97,	183.22,
      53.24,	55.39,	179.16,	183.38,
      55.39,	57.55,	179.36,	183.54,
      57.55,	59.70,	179.56,	183.70,
      59.70,	61.86,	179.77,	183.87,
      61.86,	64.01,	179.97,	184.03  ],


#5 plate    
      [ 26.60 ,  28.96 ,  175.71 , 182.49 ,                    
	28.96 ,  31.32 ,  175.95 , 182.40 ,                   
	31.32 ,  33.68 ,  176.20 , 182.32 ,                   
	33.68 ,  36.04 ,  176.46 , 182.27 ,                   
	36.04 ,  38.40 ,  176.73 , 182.23 ,                   
	38.40 ,  40.76 ,  177.02 , 182.22 ,                   
	40.76 ,  43.12 ,  177.31 , 182.22 ,                   
	43.12 ,  45.48 ,  177.62 , 182.23 ,                   
	45.48 ,  47.84 ,  177.93 , 182.28 ,                   
	47.84 ,  50.20 ,  178.15 , 182.46 ,                   
	50.20 ,  52.56 ,  178.36 , 182.63 ,                   
	52.56 ,  54.92 ,  178.58 , 182.81 ,                   
	54.92 ,  57.28 ,  178.80 , 182.98 ,                   
	57.28 ,  59.64 ,  179.01 , 183.16 ,                   
	59.64 ,  62.00 ,  179.23 , 183.33 ,                   
	62.00 ,  64.36 ,  179.45 , 183.51 ,                   
	64.36 ,  66.72 ,  179.67 , 183.68 ,                   
	66.72 ,  69.08 ,  179.88 , 183.85 ,                   
	69.08 ,  71.44 ,  180.10 , 184.03 ,                   
	71.44 ,  73.80 ,  180.32 , 184.21	],  

#plate 6: I extrapolated for new z locations. Rakitha Thu Aug 29 15:05:57 EDT 2013
    [  32.22, 34.78, 175.54, 182.21,
       34.78,	37.34,	175.81,	182.01,
       37.34,	39.91,	176.07,	181.94,
       39.91,	42.47,	176.35,	181.95,
       42.47,	45.03,	176.63,	181.98,
       45.03,	47.60,	176.92,	182.02,
       47.60,	50.16,	177.22,	182.10,
       50.16,	52.72,	177.52,	182.20,
       52.72,	55.29,	177.82,	182.32,
       55.29,	57.85,	178.11,	182.47,
       57.85,	60.41,	178.41,	182.63,
       60.41,	62.98,	178.69,	182.81,
       62.98,	65.54,	178.95,	182.98,
       65.54,	68.10,	179.18,	183.17,
       68.10,	70.67,	179.41,	183.35,
       70.67,	73.23,	179.65,	183.54,
       73.23,	75.79,	179.88,	183.73,
       75.79,	78.36,	180.13,	183.93,
       78.36,	80.92,	180.37,	184.13,
       80.92,	83.48,	180.62,	184.33  ],
                
#7 plate
      [ 37.90  , 40.67  , 175.38 , 182.05 ,
	40.67  , 43.44  , 175.66 , 182.04 ,
	43.44  , 46.21  , 175.95 , 182.04 ,
	46.21  , 48.98  , 176.25 , 182.05 ,
	48.98  , 51.75  , 176.57 , 182.08 ,
	51.75  , 54.52  , 176.89 , 182.13 ,
	54.52  , 57.29  , 177.22 , 182.19 ,
	57.29  , 60.06  , 177.57 , 182.27 ,
	60.06  , 62.83  , 177.92 , 182.36 ,
	62.83  , 65.60  , 178.29 , 182.47 ,
	65.60  , 68.37  , 178.66 , 182.66 ,
	68.37  , 71.14  , 178.90 , 182.84 ,
	71.14  , 73.91  , 179.15 , 183.02 ,
	73.91  , 76.68  , 179.39 , 183.22 ,
	76.68  , 79.45  , 179.65 , 183.42 ,
	79.45  , 82.22  , 179.90 , 183.62 ,
	82.22  , 84.99  , 180.16 , 183.84 ,
	84.99  , 87.76  , 180.43 , 184.06 ,
	87.76  , 90.53  , 180.71 , 184.28 ,
	90.53  , 93.30  , 180.98 , 184.51	],

#plate 8: I extrapolated for new z locations. Rakitha Thu Aug 29 15:05:57 EDT 2013
    [ 43.52, 46.49,	175.27,	181.86,
      46.49,	49.46,	175.56,	181.83,
      49.46,	52.43,	175.87,	181.85,
      52.43,	55.40,	176.19,	181.88,
      55.40,	58.38,	176.51,	181.93,
      58.38,	61.35,	176.86,	182.00,
      61.35,	64.32,	177.20,	182.09,
      64.32,	67.29,	177.56,	182.20,
      67.29,	70.26,	177.92,	182.32,
      70.26,	73.24,	178.28,	182.48,
      73.24,	76.21,	178.65,	182.66,
      76.21,	79.18,	179.00,	182.85,
      79.18,	82.15,	179.32,	183.06,
      82.15,	85.12,	179.58,	183.27,
      85.12,	88.10,	179.85,	183.48,
      88.10,	91.07,	180.13,	183.70,
      91.07,	94.04,	180.40,	183.92,
      94.04,	97.01,	180.69,	184.15,
      97.01,	99.98,	180.97,	184.38,
      99.98,	102.95,	181.26,	184.62 ],

#9 plate
      [ 49.20  , 52.38  , 175.06 , 181.61 ,
	52.38  , 55.56  , 175.37 , 181.64 ,
	55.56  , 58.74  , 175.70 , 181.70 ,
	58.74  , 61.92  , 176.04 , 181.76 ,
	61.92  , 65.10  , 176.39 , 181.84 ,
	65.10  , 68.28  , 176.76 , 181.94 ,
	68.28  , 71.46  , 177.13 , 182.05 ,
	71.46  , 74.64  , 177.52 , 182.17 ,
	74.64  , 77.82  , 177.91 , 182.31 ,
	77.82  , 81.00  , 178.32 , 182.46 ,
	81.00  , 84.18  , 178.75 , 182.63 ,
	84.18  , 87.36  , 179.20 , 182.85 ,
	87.36  , 90.54  , 179.48 , 183.06 ,
	90.54  , 93.72  , 179.76 , 183.28 ,
	93.72  , 96.90  , 180.05 , 183.51 ,
	96.90  ,100.08  , 180.35 , 183.75 ,
	100.08 ,103.26  , 180.65 , 183.99 ,
	103.26 ,106.44  , 180.96 , 184.24 ,
	106.44 ,109.62  , 181.27 , 184.49 ,
	109.62 ,112.80  , 181.59 , 184.75	],

#plate 10: I extrapolated for new z locations. Rakitha Thu Aug 29 15:05:57 EDT 2013
    [ 54.82, 58.20,	174.99,	181.52,
      58.20,	61.58,	175.32,	181.64,
      61.58,	64.96,	175.67, 181.76,
      64.96,	68.34,	176.03,	181.82,
      68.34,	71.72,	176.40,	181.89,
      71.72,	75.10,	176.79,	181.98,
      75.10,	78.48,	177.19,	182.08,
      78.48,	81.86,	177.60,	182.20,
      81.86,	85.24,	178.02,	182.33,
      85.24,	88.62,	178.45,	182.49,
      88.62,	92.00,	178.89,	182.69,
      92.00,	95.38,	179.31,	182.90,
      95.38,	98.76,	179.69,	183.13,
      98.76,	102.14,	179.99,	183.37,
      102.14,	105.52,	180.30,	183.61,
      105.52, 108.90,	180.61,	183.85,
      108.90,	112.28,	180.92,	184.11,
      112.28,	115.67,	181.25,	184.37,
      115.67,	119.05,	181.58,	184.64,
      119.05,	122.43,	181.91,	184.91  ],

#11 plate
      [ 60.40  , 63.98  , 174.92 , 181.35 ,
	63.98  , 67.56  , 175.27 , 181.43 ,
	67.56  , 71.14  , 175.63 , 181.52 ,
	71.14  , 74.72  , 176.00 , 181.63 ,
	74.72  , 78.30  , 176.38 , 181.75 ,
	78.30  , 81.88  , 176.78 , 181.88 ,
	81.88  , 85.46  , 177.18 , 182.02 ,
	85.46  , 89.04  , 177.60 , 182.18 ,
	89.04  , 92.62  , 178.03 , 182.35 ,
	92.62  , 96.20  , 178.47 , 182.53 ,
	96.20  , 99.78  , 178.92 , 182.73 ,
	99.78  ,103.36  , 179.39 , 182.94 ,
	103.36 , 106.94 ,  179.88,  183.19,
	106.94 , 110.52 ,  180.20,  183.43,
	110.52 , 114.10 ,  180.52,  183.69,
	114.10 , 117.68 ,  180.84,  183.94,
	117.68 , 121.26 ,  181.17,  184.21,
	121.26 , 124.84 ,  181.51,  184.48,
	124.84 , 128.42 ,  181.86,  184.76,
	128.42 , 132.00 ,  182.21,  185.04	]
)


####################################################################

# Virtual detector in front of each baffle
detthick = 0.0001

motherdepth = (z2+1.0)-(z_baf[0]-bafdepth/2)+2.0*detthick

# lab coordinate z of center of mother volume
# This needs to agree with main gdml coordinate
mother_z0 = ( (z2+1.0)+(z_baf[0]-bafdepth/2) )/2


####################################################################

header = """<?xml version="1.0" encoding="UTF-8"?>
<gdml xmlns:gdml="http://cern.ch/2001/Schemas/GDML" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="schema/gdml.xsd">
    <!-- Back GEMs -->

        <define>
	   <position name="frontgem_origin" x="0.0" y="0.0" z="0.0" unit="cm"/>
	   <rotation name="identity"/>
	   <variable name="i0" value="1"/>
	   <variable name="i1" value="1"/>"""

print header

print "\t   <constant name=\"frontgem_z1\" value=\"%f\"/>" % z1
print "\t   <constant name=\"frontgem_z2\" value=\"%f\"/>" % z2
print "\t   <constant name=\"frontgem_rmin1\" value=\"%f\"/>" % r1min
print "\t   <constant name=\"frontgem_rmax1\" value=\"%f\"/>" % r1max
print "\t   <constant name=\"frontgem_rmin2\" value=\"%f\"/>" % r2min
print "\t   <constant name=\"frontgem_rmax2\" value=\"%f\"/>" % r2max

print "        </define>"


print """        <materials>
	   <material Z="1" name="Vacuum" state="gas">
	       <T unit="K" value="2.73"/>
  	       <P unit="pascal" value="3e-18"/>
	       <D unit="g/cm3" value="1e-25"/>
	       <atom unit="g/mole" value="1.01"/>
	   </material>

	   <material name="Nitrogen" Z="7">
	       <D    unit="g/L" value="1.165"/>
	       <atom unit="g/mole" value="14.0067"/>
	   </material>
	   <material name="Oxygen" Z="8">
	       <D    unit="g/L" value="1.332"/>
	       <atom unit="g/mole" value="16"/>
	   </material>
	   <material name="Carbon" Z="6">
	       <D    unit="g/cm3" value="2.21"/>
	       <atom unit="g/mole" value="12.011"/>
	   </material>
	   <material name="Hydrogen" Z="1">
	       <D    unit="g/cm3" value="0.067"/>
	       <atom unit="g/mole" value="1.01"/>
	   </material>
	   <material name="Silicon" Z="14">
	       <D    unit="g/cm3" value="2.329"/>
	       <atom unit="g/mole" value="28.086"/>
	   </material>
	   <material name="Chlorine" Z="17">
	       <D    unit="g/cm3" value="1.574"/>
	       <atom unit="g/mole" value="35.453"/>
	   </material>

	   <material name="Air" state="gas">
	       <D value="0.00129" />
	       <fraction n="0.7" ref="Nitrogen" />
	       <fraction n="0.3" ref="Oxygen" />
	   </material>

	   <material name="NEMAG10" state="solid">
	       <D unit="g/cm3" value="1.7"/>
	       <composite n="1" ref="Silicon"/>
	       <composite n="2" ref="Oxygen"/>
	       <composite n="3" ref="Carbon"/>
	       <composite n="4" ref="Hydrogen"/>
	   </material>

	   <material name="NOMEX_pure" state="solid">
	       <D unit="g/cm3" value="1.38"/>
	       <fraction n="0.23" ref="Chlorine"/>
	       <fraction n="0.09" ref="Nitrogen"/>
	       <fraction n="0.10" ref="Oxygen"/>
	       <fraction n="0.54" ref="Carbon"/>
	       <fraction n="0.04" ref="Hydrogen"/>
	   </material>

	   <material name="NOMEX" state="solid">
	       <D unit="g/cm3" value="0.04"/>
	       <fraction n="0.45" ref="NOMEX_pure"/>
	       <fraction n="0.55" ref="Air"/>
	   </material>

	   <material  name="Kapton" state="solid">
	       <D unit="g/cm3" value="1.42"/>
	       <fraction n="0.026" ref="Hydrogen"/>
	       <fraction n="0.692" ref="Carbon"/>
	       <fraction n="0.073" ref="Nitrogen"/>
	       <fraction n="0.209" ref="Oxygen"/>
	   </material>

	   <material Z="29" name="GEMCopper" state="solid">
	       <D unit="g/cm3" value="8.960"/>
	       <atom unit="g/mole" value="63.546"/>
	   </material>

	   <material Z="18" name="Argon" state="gas">
	       <D unit="g/L" value="1.662"/>
	       <atom unit="g/mole" value="39.948"/>
	   </material>

	   <material  name="CO2">
	       <D unit="g/cm3" value="1.563"/>
	       <composite n="1" ref="Carbon"/>
	       <composite n="2" ref="Oxygen"/>
	   </material>

	   <material name="GEMgas" state="gas">
	       <D unit="g/cm3" value="1e-25"/>
	       <fraction n="0.713" ref="Argon"/>
	       <fraction n="0.287" ref="CO2"/>
	   </material>

	   <material Z="82" name="Lead" state="solid">
	       <D unit="g/cm3" value="11.350"/>
	       <atom unit="g/mole" value="207.2"/>
	   </material>

	   <material Z="82" name="Kryptonite" state="solid">
	       <D unit="g/cm3" value="11.350"/>
	       <atom unit="g/mole" value="207.2"/>
	   </material>

        </materials>
	
	<solids>"""
print """	      <cone name="frontgemvol" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin1="%f" rmax1="%f" rmin2="%f" rmax2="%f"  z="%f" />""" % (piperad1, outrad, piperad2, outrad, motherdepth)

print """	     <!--  Main 23 GEM layers -->"""

for i in range(nlayer):
    print "	         <tube name=\"frontgemlay1_%02d\" aunit=\"deg\" startphi=\"-5\" deltaphi=\"10\" lunit=\"cm\" rmin=\"frontgem_rmin1\" rmax=\"frontgem_rmax1\" z=\"%f\" />" % (i, gemthick[i])

for i in range(nlayer):
    print "	         <tube name=\"frontgemlay2_%02d\" aunit=\"deg\" startphi=\"-5\" deltaphi=\"10\" lunit=\"cm\" rmin=\"frontgem_rmin2\" rmax=\"frontgem_rmax2\" z=\"%f\" />" % (i, gemthick[i])

print """	     <!-- Baffle rings/dets -->"""

for i in range(nbaf):
    print """	         <tube name="bafring_in_%02d" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%f" rmax="%f" z="%f" />""" % (i, rin_inbaf[i], rout_inbaf[i],  bafdepth)
    print """	         <tube name="bafring_out_%02d" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%f" rmax="%f" z="%f" />""" % (i, rin_outbaf[i], rout_outbaf[i],  bafdepth)
    print """	         <tube name="bafdet_%02d" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%f" rmax="%f" z="%f" />""" % (i, rout_inbaf[i], rin_outbaf[i], detthick)


print """	     <!-- Baffle block -->"""

for i in range(nbaf):
      for j in range(nblock):
          # compute the start phi and delta phi correctly for babar baffles : rakitha Fri Oct 25 09:22:09 EDT 2013
	  sphi = mybaf[i][j*4+2]
	  dphi = mybaf[i][j*4+3]-sphi+1 #1 deg more
############### 
	  # startphi = sphi + dphi
	  # stopphi  = sphi + 360/nsector
	  # phidiff  = stopphi - startphi
###############
          startphi = sphi
          phidiff = dphi

	  print """	         <tube name="bafblock_%02d_%02d" aunit="deg" startphi="%f" deltaphi="%f" lunit="cm" rmin="%f" rmax="%f" z="%f" />""" % (i, j, startphi, phidiff, mybaf[i][j*4], mybaf[i][j*4+1],  bafdepth)




print """        </solids>

	<structure>
"""

# Make logical volumes
# We need unique volumes for detector layers, but not for non-detectors so
# that we can assign detector numbers
for i in range(nlayer):
    #  Non detector layer
    if not i in detlayer:
	####  First plane
	print """	         <volume name="logicfrontgemlay1_%02d">
		      <materialref ref =\"%s\"/> 
		      <solidref ref =\"frontgemlay1_%02d\"/>"""  % (i, gemmat[i], i)
        if i == 0 or i == nlayer-1: 
	    print """		      <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Magenta"/>"""
        else:
	    print """		      <auxiliary auxtype="Visibility" auxvalue="false"/>"""
        print """		   </volume>"""
	####  Second plane
	print """	         <volume name="logicfrontgemlay2_%02d">
		      <materialref ref =\"%s\"/> 
		      <solidref ref =\"frontgemlay2_%02d\"/>"""  % (i, gemmat[i], i)
        if i == 0 or i == nlayer-1: 
	    print """		      <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Magenta"/>"""
        else:
	    print """		      <auxiliary auxtype="Visibility" auxvalue="false"/>"""
        print """		   </volume>"""
    else: 
    #  Detector layers
	for j in range(nsector):
	    detno = frontgemdet1no + j + 100*i
	    #  First plane
	    print """	         <volume name="logicfrontgemlay1_%02d_%02d">
		      <materialref ref =\"%s\"/> 
		      <solidref ref =\"frontgemlay1_%02d\"/>"""  % (i, j, gemmat[i], i)
	    print """		      <auxiliary auxtype="SensDet" auxvalue="planeDet"/>
		      <auxiliary auxtype="DetNo" auxvalue="%d"/>
	          </volume>""" % detno
	    #  Second plane
	    detno = frontgemdet2no + j + 100*i
	    print """	         <volume name="logicfrontgemlay2_%02d_%02d">
		      <materialref ref =\"%s\"/> 
		      <solidref ref =\"frontgemlay2_%02d\"/>"""  % (i, j, gemmat[i], i)
	    print """		      <auxiliary auxtype="SensDet" auxvalue="planeDet"/>
		      <auxiliary auxtype="DetNo" auxvalue="%d"/>
                  </volume>""" % detno

for i in range(nbaf):
    	    #  Inner baffle ring
	    print """	         <volume name="logicbafring_in_%02d">
		      <materialref ref ="%s"/> 
		      <solidref ref ="bafring_in_%02d"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Cyan"/>
		      </volume>""" % (i, bafmat, i)
    	    #  Outer baffle ring
	    print """	         <volume name="logicbafring_out_%02d">
		      <materialref ref ="%s"/> 
		      <solidref ref ="bafring_out_%02d"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Cyan"/>
		      </volume>""" % (i, bafmat, i)
	    # Detector for baffle
	    print """	         <volume name="logicbafdet_%02d">
		      <materialref ref ="%s"/> 
		      <solidref ref ="bafdet_%02d"/>
	              <auxiliary auxtype="Visibility" auxvalue="false"/>
		      </volume>""" % (i, "Air", i)

for i in range(nbaf):
      for j in range(nblock):
	    print """	         <volume name="logicbafblock_%02d_%02d">
		      <materialref ref ="%s"/> 
		      <solidref ref ="bafblock_%02d_%02d"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Cyan"/>
		      </volume>""" % (i,j, bafmat, i, j)

print """
        <!---  Place volumes -->
	        <volume name="logicGEMbafflesMother">
		      <materialref ref="Vacuum"/>
		      <solidref ref="frontgemvol"/>
	              <auxiliary auxtype="Visibility" auxvalue="false"/>
    """

print """	        <loop for=\"i0\" to=\"%d\" step=\"1\">""" % nsector

#######  GEM positioning
thisz = 0.0
for i in range(nlayer):
    if i > 0:
    	thisz += gemthick[i-1]/2.0

    thisz += gemthick[i]/2.0
    #  Non detector layer
    if not i in detlayer:
        print	"""                   <physvol>
                        <volumeref ref="logicfrontgemlay1_%02d"/>
		        <position name="frontgempos1_%02d" unit="cm" x="0" y="0" z="%f"/>
		        <rotation name="frontgemrot1_%02d" unit="deg" x="0" y="0" z="360.0*i0/%d+%f"/>
                   </physvol>""" % (i, i, thisz + z1 - mother_z0, i, nsector, rot1)
        print	"""                   <physvol>
                        <volumeref ref="logicfrontgemlay2_%02d"/>
		        <position name="frontgempos2_%02d" unit="cm" x="0" y="0" z="%f"/>
		        <rotation name="frontgemrot2_%02d" unit="deg" x="0" y="0" z="360.0*i0/%d+%f"/>
                   </physvol>""" % (i, i, thisz + z2 - mother_z0, i, nsector, rot2)

print """	        </loop>"""
	
for k in range(len(detlayer)):
  # Calculate detector layer zposition
  thisz = 0.0
  for i in range(detlayer[k]+1):
    if i > 0:
    	thisz += gemthick[i-1]/2.0
    thisz += gemthick[i]/2.0

  for j in range(nsector):
        print	"""                <physvol>
                   <volumeref ref="logicfrontgemlay1_%02d_%02d"/>
		   <position name="frontgempos1_%02d_%02d" unit="cm" x="0" y="0" z="%f"/>
		   <rotation name="frontgemrot1_%02d_%02d" unit="deg" x="0" y="0" z="360.0*%0.1f/%d+%f"/>
                </physvol>""" % (detlayer[k], j, detlayer[k], j, thisz + z1 - mother_z0, detlayer[k], j, float(j), nsector, rot1)
        print	"""                <physvol>
                   <volumeref ref="logicfrontgemlay2_%02d_%02d"/>
		   <position name="frontgempos2_%02d_%02d" unit="cm" x="0" y="0" z="%f"/>
		   <rotation name="frontgemrot2_%02d_%02d" unit="deg" x="0" y="0" z="360.0*%0.1f/%d+%f"/>
                </physvol>""" % (detlayer[k], j, detlayer[k], j, thisz + z2 - mother_z0, detlayer[k], j, float(j), nsector, rot2)

#######  Baffle Ring/Detector positioning
for i in range(nbaf):
        print	"""                <physvol>
                   <volumeref ref="logicbafring_in_%02d"/>
		   <position name="bafringinpos1_%02d" unit="cm" x="0" y="0" z="%f"/>
                </physvol>""" % (i, i, z_baf[i] - mother_z0 )
        print	"""                <physvol>
                   <volumeref ref="logicbafring_out_%02d"/>
		   <position name="bafringoutpos1_%02d" unit="cm" x="0" y="0" z="%f"/>
                </physvol>""" % (i, i, z_baf[i] - mother_z0 )
        print	"""                <physvol>
                   <volumeref ref="logicbafdet_%02d"/>
		   <position name="bafdetpos1_%02d" unit="cm" x="0" y="0" z="%f"/>
                </physvol>""" % (i, i, z_baf[i] - bafdepth/2.0 - detthick/2 - mother_z0 )


#######  Baffle block positioning 
#rot_offset added to make proper rotation : rakitha Thu Oct 10 14:11:58 EDT 2013
print """	        <loop for=\"i1\" to=\"%d\" step=\"1\">""" % nsector
for i in range(nbaf):
      for j in range(nblock):
	  print	"""                    <physvol>
                       <volumeref ref="logicbafblock_%02d_%02d"/>
		       <position name="bafblockpos_%02d_%02d" unit="cm" x="0" y="0" z="%f"/>
		       <rotation name="bafblockrot_%02d_%02d" unit="deg" x="0" y="0" z="(i1-1)*360.0/%d-%f"/>
                    </physvol>""" % (i, j, i, j, z_baf[i] - mother_z0,  i,j,nsector,rot_offset[i] )

print """	        </loop>"""

print """
	        </volume>
	</structure>

	<setup name="GEMbaffles" version="1.0">
	     <world ref="logicGEMbafflesMother"/>
	</setup>
</gdml>"""








