#!/usr/bin/python

import cerhelp

from ROOT import TVector3, TRotation
from math import sin, cos, asin, acos, fabs, pi, sqrt, tan

nsector = 30

cerdetno = 70000

motherrmin = 60
motherrmax = 265
motherz    = 301-194+1

# PVDIS LG Cerenkov parameters
# Based on work by Michael Paolone

##########  Tank and Windows ####################################

tankz    = ( 194,  209.01,  209.01,  301 )
tankrmin = ( 65, 67, 67, 85 )
tankrmax = ( 144, 155, 265, 265 )

cleo_fw_thk = 0.005 #cm
fwinrmin = 65.1
fwinrmax = 143.9
fwz = tankz[0]-cleo_fw_thk/2

cleo_bw_thk = 0.010 #cm
bwinrmin = 85.1
bwinrmax = 264.9
bwz = tankz[-1]+cleo_bw_thk/2

tankdepth = tankz[-1]-tankz[0]+1.0

#  This needs to agree with main definition
cerz0 = (tankz[-1]+tankz[0])/2

##########  PMTS ################################################

pmtx_ang = 63.0*pi/180;
pmty_ang =  0.0*pi/180;
pmtz_ang =  1.8*pi/180;

image_x = 0.0
image_y = 240.0
image_z = 240.0

PMT_hflngth = 2.54
PMTwindow_hfthk = 0.075

PMT_width = 2.6
PMT_shell = 0.0001

PMT_depth = 0.00001

pmtN = 3  # pmtN x pmtN array

Pos_im_Obs_V = TVector3( image_x, image_y, image_z )

Pos_obs_V = Pos_im_Obs_V

Pos_obsWin_V = TVector3(0.0, 0.0, -PMTwindow_hfthk)

##########  Cones ###############################################

wincone_len = 30.0

wincone_endrmin = 21.0
wincone_endrmax = 21.5

pmt_shield_hlength = 10.0

crossl = sqrt(2.0)*(pmtN*PMT_width+0.5) - pmtN*PMT_width
cyl_dist = -pmt_shield_hlength - wincone_len/2 + crossl/( ( wincone_endrmax - (pmtN*PMT_width+0.5))/(2.0*wincone_len/2))
shield_offV = TVector3( 0.0, 0.0, cyl_dist )

Pos_winstonCone_V = Pos_im_Obs_V;

##########  Mirrors #############################################

Z_M1        = 270.0; #cm  Mirror z-cord w.r.t origin
T_M1        = 0.300; #cm  Mirror thickness
cr_ang1     = 11.3;  #deg  Mirror central angle;
ztarg_cent1 = -350; #Nominal target center for mirror 1
Angle_in1   =  7.6; #deg  Defines inner radius of mirror array
Angle_out1  = 15.0;  #deg  Defines outer radius of mirror array
mirrAng1    = -7.0;  #deg  For rotation of mirror
zMirrRotPoint1 = Z_M1 - 39.0; #point at which you wish to rotate mirror about in Z;


Z_M2        = 293.0; #cm  Mirror z-cord w.r.t origin
T_M2        = 0.3; #cm  Mirror thickness
cr_ang2     = 33.5;  #deg  Mirror central angle;
ztarg_cent2 = 10.0; #Nominal target center for mirror 1
Angle_in2   = 32.3; #deg  Defines inner radius of mirror array
Angle_out2  = 38.0;  #deg  Defines outer radius of mirror array
mirrAng2    = 0.0;  #deg  For rotation of mirror
zMirrRotPoint2 = Z_M2 - 30.0; #point at which you wish to rotate mirror about in Z;


####################################################################
# Intermediate calculations
tankparams = []
for i in range(len(tankz)):
    tankparams.append(tankrmin[i])
    tankparams.append(tankrmax[i])
    tankparams.append(tankz[i]-cerz0)

motherparams = []
motherparams.append(tankrmin[0])
motherparams.append(tankrmin[0])
motherparams.append(tankz[0]-cerz0 - cleo_bw_thk)
motherparams += tankparams
motherparams.append(tankrmin[-1])
motherparams.append(tankrmin[-1])
motherparams.append(tankz[-1]-cerz0 + cleo_bw_thk)

mir_rotz = TRotation()
mir_rotz.RotateZ(2.0*pi/nsector)  #initialize it so we have zero rotation when we start

pmt_rotz = TRotation()
pmt_rotx = TRotation()
pmt_roty = TRotation()
pmt_rotz.RotateZ(pmtz_ang)
pmt_roty.RotateY(pmty_ang)
pmt_rotx.RotateX(pmtx_ang)

Pos_obs_hfloff = TVector3(0.0, 0.0, -PMT_hflngth)
Pos_obs_hfloff = pmt_roty*Pos_obs_hfloff
Pos_obs_hfloff = pmt_rotx*Pos_obs_hfloff

Pos_obs_V = Pos_obs_V + Pos_obs_hfloff

Pos_winstonCone_off_V = TVector3(0.0,0.0, wincone_len/2)
Pos_winstonCone_off_V = pmt_roty*Pos_winstonCone_off_V
Pos_winstonCone_off_V = pmt_rotx*Pos_winstonCone_off_V

Pos_winstonCone_V = Pos_winstonCone_V + Pos_winstonCone_off_V

shield_offV = pmt_roty*shield_offV
shield_offV = pmt_rotx*shield_offV

if pmtN > 3:
    sys.stderr.write("Error:  the counting scheme implemented requires there are less than 10 PMTs per sector")
    exit(1)

#  Mirror calcs ###########################################################################

   # Mirror 1 #######

V0_tg_V = TVector3( 0.0, 0.0, ztarg_cent1 )
mirrRotPoint = TVector3(0.0, (-V0_tg_V.Z() + zMirrRotPoint1)*tan(cr_ang1*pi/180), zMirrRotPoint1)
Z1_front = 200.0 - V0_tg_V.Z()
Z1_end   = 301.0 - V0_tg_V.Z()

R1_front_in  = Z1_front*tan(Angle_in1*pi/180);
R1_front_out = Z1_front*tan(Angle_out1*pi/180);
R1_end_in    = Z1_end*tan(Angle_in1*pi/180);
R1_end_out   = Z1_end*tan(Angle_out1*pi/180);

RposV = cerhelp.buildSPmirror( V0_tg_V, Pos_im_Obs_V, cr_ang1, Z_M1)

#print RposV[0], " at "
#RposV[1].Print()
#exit(1)

R = RposV[0]
PosV1 = RposV[1]
mirror1R = RposV[0]

PosV1_temp   = PosV1;
PosV1_cone   = TVector3(0.,0.,250.5);
PosV_delta  = TVector3(0., 0., 0.);
PosV_deltaC = TVector3(0., 0., 0.);

if mirrAng1 != 0.0:
    PosV_delta  = cerhelp.mirrorRot(PosV1, RposV[0], cr_ang1, PosV1_temp, mirrAng1, ztarg_cent1, zMirrRotPoint1)
    PosV1_temp   = PosV1_temp + PosV_delta
    PosV_deltaC = cerhelp.mirrorRot(PosV1, RposV[0], cr_ang1, PosV1_cone, mirrAng1, ztarg_cent1, zMirrRotPoint1)
    PosV1_cone   = PosV1_cone + PosV_deltaC

PosV1_temp_CM = PosV1_temp



RotPointV = TVector3( 0.0, PosV1.y() - sqrt(R*R - (zMirrRotPoint1 - PosV1.Z())*(zMirrRotPoint1 - PosV1.Z())), zMirrRotPoint1)

cerhelp.calcIntPoints( R, PosV1_temp, PosV1_cone, 0.5*(Z1_end - Z1_front), R1_front_in, R1_front_out, R1_end_in, R1_end_out, -mirrAng1)


   # Mirror 2 #######

V0_tg_V = TVector3( 0.0, 0.0, ztarg_cent2 )
mirrRotPoint = TVector3(0.0, (-V0_tg_V.Z() + zMirrRotPoint2)*tan(cr_ang2*pi/180), zMirrRotPoint2)
Z2_front = 200.0 - V0_tg_V.Z()
Z2_end   = 301.0 - V0_tg_V.Z()

R2_front_in  = Z2_front*tan(Angle_in2*pi/180);
R2_front_out = Z2_front*tan(Angle_out2*pi/180);
R2_end_in    = Z2_end*tan(Angle_in2*pi/180);
R2_end_out   = Z2_end*tan(Angle_out2*pi/180);

RposV = cerhelp.buildSPmirror( V0_tg_V, Pos_im_Obs_V, cr_ang2, Z_M2)

R = RposV[0]
PosV2 = RposV[1]
mirror2R = RposV[0]

PosV2_temp   = PosV2;
PosV2_cone   = TVector3(0.,0.,250.5);
PosV_delta  = TVector3(0., 0., 0.);
PosV_deltaC = TVector3(0., 0., 0.);

if mirrAng2 != 0.0:
    PosV_delta  = cerhelp.mirrorRot(PosV2, RposV[0], cr_ang2, PosV2_temp, mirrAng2, ztarg_cent2, zMirrRotPoint2)
    PosV2_temp   = PosV2_temp + PosV_delta
    PosV_deltaC = cerhelp.mirrorRot(PosV2, RposV[0], cr_ang2, PosV2_cone, mirrAng2, ztarg_cent2, zMirrRotPoint2)
    PosV2_cone   = PosV2_cone + PosV_deltaC

PosV2_temp_CM = PosV2_temp
RotPointV = TVector3( 0.0, PosV2.y() - sqrt(R*R - (zMirrRotPoint2 - PosV2.Z())*(zMirrRotPoint2 - PosV2.Z())), zMirrRotPoint2)

cerhelp.calcIntPoints( R, PosV2_temp, PosV2_cone, 0.5*(Z2_end - Z2_front), R2_front_in, R2_front_out, R2_end_in, R2_end_out, -mirrAng2)

####################################################################

header = """<?xml version="1.0" encoding="UTF-8"?>
<gdml xmlns:gdml="http://cern.ch/2001/Schemas/GDML" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="schema/gdml.xsd">
    <!-- HG Cereknov -->

        <define>
	   <position name="cerenkov_origin" x="0.0" y="0.0" z="0" unit="cm"/>
	   <rotation name="identity"/>
	   <variable name="i0" value="1"/>"""

print header

print "\t   <constant name=\"certank_z1\" value=\"%f\"/>" % tankz[0]

print "           <matrix name=\"cergas_ridx\" coldim=\"2\" values=\"" + cerhelp.arrangetables("""2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  1.000988059 1.000988436 1.000988837 1.000989683 1.000990129 1.000990829 1.000991318 1.000991825 1.000992899 1.000993468 1.000994060 1.000994676 
  1.000995317 1.000995984 1.000997014 1.000998098 1.000998848 1.001000480 1.001001340 1.001002710 1.001004180 1.001006320 1.001008640 1.001010570 
  1.001013460 1.001017550 1.001021260 1.001025430 1.00103013 1.00103568 1.00104007 1.00104498 1.00105229 1.00105853 1.00106803 1.00107341 
  1.00107926 1.00108554 1.0011001 1.00111275 1.00112755""") +  "\"/>"

print "           <matrix name=\"cergas_abs\" coldim=\"2\" values=\"" + cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  135.2*m 135.195*m 135.19*m 135.178*m 135.172*m 135.161*m 135.154*m 135.145*m 135.127*m 135.117*m 135.106*m 135.094*m 
  135.081*m 135.068*m 134.818*m 139.371*m 144.204*m 157.01*m 163.947*m 174.23*m 182.697*m 188.896*m 188.17*m 183.538*m 
  173.753*m 160.211*m 150.994*m 144.181*m 139.952*m 137.75*m 136.876*m 135.607*m 131.482*m 124.93*m 110.337*m 100.711*m 
  90.1471*m 79.394*m 58.692*m 45.7222*m 35.0874*m""") + "\"/>"


print "            <matrix name=\"cerair_eff\" coldim=\"2\" values=\" "+ cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 
  1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0""") + "\"/>"

print "            <matrix name=\"cerair_ridx\" coldim=\"2\" values=\""+  cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 
  1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5 1.5""") + "\"/>"


print "           <matrix name=\"cerwin_ridx\" coldim=\"2\" values=\"" + cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
    1.51568 1.513 1.51024 1.50472 1.50196 1.49783 1.49507 1.49231 1.48679 1.48403 1.48128 1.47852 1.47576 1.473 1.46901 1.46995 
    1.46667 1.46799 1.46733 1.46767 1.46934 1.47201 1.47434 1.47838 1.4868 1.49445 1.5008 1.50321 1.504 1.504 1.504 1.504 
    1.504 1.504 1.504 1.504 1.504 1.504 1.504 1.504 1.504""") + "\"  />"

print "           <matrix name=\"cercath_eff\" coldim=\"2\" values=\"" +  cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
       0.008 0.0124 0.0157 0.02125 0.0275 0.034 0.04 0.048 0.062 0.0753 0.09 0.1071 0.12144 0.1428 0.15 0.16429 
       0.17857 0.1928 0.2 0.2125 0.225 0.2375 0.25 0.2625 0.275 0.275 0.275 0.275 0.275 0.275 0.2625 0.25 0.2375 0.2125 0.192859 0.185716 
       0.178573 0.15714 0.13572 0.1143 0.09""") + "\"  />"

print "           <matrix name=\"cercath_ridx\" coldim=\"2\" values=\" "+cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 
  1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0""")+ "\"  />"

print "           <matrix name=\"cercath_rfct\" coldim=\"2\" values=\"" + cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 
  0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0""") + "\" />"

print "           <matrix name=\"cermir_rfct\" coldim=\"2\" values=\"" + cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 
  0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85 0.85""") + "\"  />"

print "           <matrix name=\"cermir_specspikecst\" coldim=\"2\" values=\"" + cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 
  0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 
  0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025""") + "\" />"

print "           <matrix name=\"cermir_backsctcst\" coldim=\"2\" values=\"" +  cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 
  0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 
  0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025 0.0025""") + "\" />"

print "           <matrix name=\"cermir_speclobecst\" coldim=\"2\" values=\"" + cerhelp.arrangetables(""" 2.04358*eV 2.06640*eV 2.09046*eV 2.14023*eV 2.16601*eV 2.20587*eV 2.23327*eV 2.26137*eV 2.31972*eV 2.35005*eV 2.38116*eV 2.41313*eV 
  2.44598*eV 2.47968*eV 2.53081*eV 2.58354*eV 2.61940*eV 2.69589*eV 2.73515*eV 2.79685*eV 2.86139*eV 2.95271*eV 3.04884*eV 3.12665*eV 
  3.23930*eV 3.39218*eV 3.52508*eV 3.66893*eV 3.82396*eV 3.99949*eV 4.13281*eV 4.27679*eV 4.48244*eV 4.65057*eV 4.89476*eV 5.02774*eV 
  5.16816*eV 5.31437*eV 5.63821*eV 5.90401*eV 6.19921*eV
  0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 
  0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 
  0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99""") +"\" />"

print "        </define>"


print """        <materials>
	   <material Z="1" name="Vacuum" state="gas">
	       <T unit="K" value="2.73"/>
  	       <P unit="pascal" value="3e-18"/>
	       <D unit="g/cm3" value="1e-25"/>
	       <atom unit="g/mole" value="1.01"/>
	   </material>
	   <material Z="13" name="Aluminum" state="solid">
	        <D unit="g/cm3" value="2.70"/>
	        <atom unit="g/mole" value="26.98"/>
	   </material>
	   <material name="Nickel" Z="28">
	       <D    unit="g/cm3" value="8.908"/>
	       <atom unit="g/mole" value="58.693"/>
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
	   <material name="Boron" Z="5">
	       <D    unit="g/cm3" value="2.08"/>
	       <atom unit="g/mole" value="10.81"/>
	   </material>
	   <material name="Sodium" Z="11">
	       <D    unit="g/cm3" value="0.927"/>
	       <atom unit="g/mole" value="22.989"/>
	   </material>
	   <material name="Fluorine" Z="9">
	       <D    unit="g/L" value="1.696"/>
	       <atom unit="g/mole" value="18.998"/>
	   </material>
	   <material name="Iron" Z="28">
	       <D    unit="g/cm" value="7.874"/>
	       <atom unit="g/mole" value="55.845"/>
	   </material>
	   <material name="Molybdenum" Z="42">
	       <D    unit="g/cm" value="10.28"/>
	       <atom unit="g/mole" value="95.96"/>
	   </material>

	   <material name="Air" state="gas">
	       <D value="0.00129" />
	       <fraction n="0.7" ref="Nitrogen" />
	       <fraction n="0.3" ref="Oxygen" />
	   </material>
	   <material  name="CO2">
	       <D unit="g/cm3" value="1.563"/>
	       <composite n="1" ref="Carbon"/>
	       <composite n="2" ref="Oxygen"/>
	   </material>
	   <material  name="H8500_PMToption">
		  <property name="EFFICIENCY" ref="cercath_eff" />
		  <property name="RINDEX" ref="cercath_ridx" />
		  <property name="REFLECTIVITY" ref="cercath_rfct" />
	          <D unit="g/cm3" value="2.203"/>
  		  <fraction n="0.33" ref="Silicon" />
	          <fraction n="0.67" ref="Oxygen" />

	   </material>
	   <material  name="H8500UVwin">
		  <property name="RINDEX" ref="cerwin_ridx" />
	          <D unit="g/cm3" value="2.320"/>
  		  <fraction n="0.25" ref="Silicon" />
	          <fraction n="0.64" ref="Oxygen" />
  		  <fraction n="0.08" ref="Boron" />
	          <fraction n="0.03" ref="Sodium" />
	   </material>
	   <material  name="PVDIS_C4F8O_N2_Gas">
		  <property name="RINDEX" ref="cergas_ridx" />
		  <property name="ABSLENGTH" ref="cergas_abs" />
	          <D unit="g/cm3" value="0.00617"/>
  		  <fraction n="0.20" ref="Carbon" />
	          <fraction n="0.40" ref="Fluorine" />
	          <fraction n="0.05" ref="Oxygen" />
	          <fraction n="0.35" ref="Nitrogen" />
	   </material>
	   <material  name="PVDIS_mirror">
		  <property name="REFLECTIVITY" ref="cermir_rfct" />
		  <property name="SPECULARSPIKECONSTANT" ref="cermir_specspikecst" />
		  <property name="SPECULARBACKSCATTERCONSTANT" ref="cermir_backsctcst" />
		  <property name="SPECULARLOBECONSTANT" ref="cermir_speclobecst" />
	          <D unit="g/cm3" value="2.203"/>
  		  <fraction n="0.33" ref="Silicon" />
	          <fraction n="0.67" ref="Oxygen" />
	   </material>
	   <material  name="mumetal">
	          <D unit="g/cm3" value="8.7"/>
  		  <fraction n="0.80" ref="Nickel" />
  		  <fraction n="0.15" ref="Iron" />
  		  <fraction n="0.05" ref="Molybdenum" />
	   </material>

        </materials>
	
	<solids>"""

print """         <polycone name="cermother" aunit="deg" startphi="0" deltaphi="360" lunit="cm">
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
        </polycone>
""" % tuple(motherparams)

print """         <polycone name="certank" aunit="deg" startphi="0" deltaphi="360" lunit="cm">
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
	       <zplane rmin="%f" rmax="%f" z="%f"/>
        </polycone>
""" % tuple(tankparams)

print """         <tube name="cerbackwin" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%f" rmax="%f" z="%f"/>""" % (bwinrmin, bwinrmax, cleo_bw_thk)
print """         <tube name="cerfrontwin" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%f" rmax="%f" z="%f"/>""" % (fwinrmin, fwinrmax, cleo_fw_thk)

print """         <box name="cerpmtcath" lunit="cm" x="%f" y="%f" z="%f"/>""" % ((PMT_width-PMT_shell)*2, (PMT_width-PMT_shell)*2, PMT_depth*2)
print """         <box name="cerpmtwin"  lunit="cm" x="%f" y="%f" z="%f"/>""" % ((PMT_width-PMT_shell)*2, (PMT_width-PMT_shell)*2, PMTwindow_hfthk*2)
print """         <cone name="cerwincone" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin1="%f" rmax1="%f" rmin2="%f" rmax2="%f" z="%f"/>""" % (pmtN*PMT_width, pmtN*PMT_width+0.5, wincone_endrmin, wincone_endrmax, wincone_len)
print """         <cone name="cerpmtshield" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin1="%f" rmax1="%f" rmin2="%f" rmax2="%f" z="%f"/>""" % (sqrt(2.0)*(pmtN*PMT_width+0.5), sqrt(2.0)*(pmtN*PMT_width+1.0), sqrt(2.0)*(pmtN*PMT_width+0.5), sqrt(2.0)*(pmtN*PMT_width+1.0), pmt_shield_hlength*2)

print """         <sphere name="cermirror1sphere" aunit="deg" startphi="0" deltaphi="360" starttheta="0" deltatheta="90" lunit="cm" rmin="%f" rmax="%f"/>""" % (mirror1R, mirror1R + T_M1 )
print """         <cone name="cermirror1cone" aunit="deg" lunit="cm" rmin1="%f" rmax1="%f" rmin2="%f" rmax2="%f" z="%f" startphi="%f" deltaphi="%f"/>""" % ( R1_front_in, R1_front_out, R1_end_in, R1_end_out, Z1_end - Z1_front, 84, 12  )
print """         <intersection name="cermirror1">
			<first ref="cermirror1sphere"/>
			<second ref="cermirror1cone"/>
			<position unit="cm" name="mirror1intpos" x="%f" y="%f" z="%f" />
		        <rotation unit="deg" name="mirror1introt" x="%f" y="%f" z="%f" /> 
		  </intersection>""" % (PosV1_cone.X()-PosV1_temp.X(), PosV1_cone.Y()-PosV1_temp.Y(), PosV1_cone.Z()-PosV1_temp.Z(),  mirrAng1, 0.0, 0.0 )

		  
print """         <sphere name="cermirror2sphere" aunit="deg" startphi="0" deltaphi="360" starttheta="0" deltatheta="90" lunit="cm" rmin="%f" rmax="%f"/>""" % (mirror2R, mirror2R + T_M2 )
print """         <cone name="cermirror2cone" aunit="deg" lunit="cm" rmin1="%f" rmax1="%f" rmin2="%f" rmax2="%f" z="%f" startphi="%f" deltaphi="%f"/>""" % ( R2_front_in, R2_front_out, R2_end_in, R2_end_out, Z2_end - Z2_front, 84, 12  )
print """         <intersection name="cermirror2">
			<first ref="cermirror2sphere"/>
			<second ref="cermirror2cone"/>
			<position unit="cm" name="mirror2intpos" x="%f" y="%f" z="%f" />
		        <rotation unit="deg" name="mirror2introt" x="%f" y="%f" z="%f" /> 
		  </intersection>""" % (PosV2_cone.X()-PosV2_temp.X(), PosV2_cone.Y()-PosV2_temp.Y(), PosV2_cone.Z()-PosV2_temp.Z(),  mirrAng2, 0.0, 0.0 )


print """   <opticalsurface name="MirrorSurf" model="unified" finish="polished" type="dielectric_metal" value="1.0"/>"""


print """        </solids>

	<structure>
"""

#  PMT logical volumes
# pmtN x pmtN array
for i in range(nsector):
    for j in range(pmtN):
	for k in range(pmtN):
               print"""	<volume name="logiccercath_%02d_%02d">
		      <materialref ref="H8500_PMToption"/>
		      <solidref ref="cerpmtcath"/>
		      <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Yellow"/>
	</volume>""" % (i, j*pmtN+k)

print"""        <volume name="logiccerpmtwin">
		      <materialref ref="H8500UVwin"/>
		      <solidref ref="cerpmtwin"/>
	              <auxiliary auxtype="Visibility" auxvalue="wireframe"/>
	              <auxiliary auxtype="Color" auxvalue="White"/>
	</volume>"""

print"""        <volume name="logiccerwincone">
		      <materialref ref="PVDIS_mirror"/>
		      <solidref ref="cerwincone"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="White"/>
	</volume>"""

print"""        <volume name="logiccerpmtshield">
		      <materialref ref="mumetal"/>
		      <solidref ref="cerpmtshield"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="Gray"/>
	</volume>"""

print"""        <volume name="logiccermirror1">
		      <materialref ref="PVDIS_mirror"/>
		      <solidref ref="cermirror1"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="Red"/>
	</volume>"""

print"""        <volume name="logiccermirror2">
		      <materialref ref="PVDIS_mirror"/>
		      <solidref ref="cermirror2"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="Green"/>
	</volume>"""

# Tank with PMTS

print"""        <volume name="logiccertank">
		      <materialref ref="PVDIS_C4F8O_N2_Gas"/>
		      <solidref ref="certank"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="Blue"/>"""

# Place PMTs
for i in range(nsector):
    pmt_rotz.RotateZ(-2.0*pi/nsector)
    mir_rotz.RotateZ(-2.0*pi/nsector)

    temp_pos_V = Pos_obs_V
    temp_posW_V = Pos_obs_V + Pos_obsWin_V
    temp_pos_V = pmt_rotz*temp_pos_V
    temp_init_V = temp_pos_V

    for j in range(pmtN):
	for k in range(pmtN):

	    if( pmtN % 2 ):
		temp_off_V = TVector3(2.0*(j -(pmtN -1)/2.0)*PMT_width,  2.0*(k -(pmtN -1)/2.0)*PMT_width, PMT_hflngth - 2*PMTwindow_hfthk- 0.01)
	    else:
		temp_off_V = TVector3((2.0*j - (pmtN -1))*PMT_width,  (2.0*k - (pmtN -1))*PMT_width, PMT_hflngth - 2*PMTwindow_hfthk - 0.01)

	    tempWindow_off_V = temp_off_V + TVector3(0,0, PMTwindow_hfthk + 0.01)

	    temp_off_V = pmt_rotx*temp_off_V
	    temp_off_V = pmt_roty*temp_off_V
	    temp_off_V = pmt_rotz*temp_off_V

	    temp_pos_V = temp_init_V + temp_off_V

	    thiszang = pmtz_ang-2.0*pi*(i+1)/nsector

#   Ugh...   GDML only does rotations in one order.  Need to change zxy -> xyz
#   This helper gets fed (th_x, th_y, th_z)  in that order   # FIXME, code up ZXY
	    newrotangs = cerhelp.ZYXrotToXYZ(-pmtx_ang, 0.0, -thiszang)

	    #  PMT cathodes
	    print	"""                <physvol>
		      <volumeref ref="logiccercath_%02d_%02d"/>
		      <position unit="cm" name="pmtcathpos_%02d_%02d" x="%f" y="%f" z="%f"/>
		      <rotation unit="rad" name="pmtcathrot_%02d_%02d" x="%f" y="%f" z="%f"/>
                </physvol>""" % (i, j*pmtN+k, i, j*pmtN+k, temp_pos_V.X(), temp_pos_V.Y(), temp_pos_V.Z() - cerz0, i, j*pmtN+k, newrotangs[0], newrotangs[1], newrotangs[2] )

	    #  PMT windows

	    tempWindow_off_V = pmt_rotx*tempWindow_off_V
	    tempWindow_off_V = pmt_rotz*tempWindow_off_V
	    tempWindow_pos_V = temp_pos_V-temp_off_V+tempWindow_off_V

	    print	"""                <physvol>
		      <volumeref ref="logiccerpmtwin"/>
		      <position unit="cm" name="pmtwinpos_%02d_%02d" x="%f" y="%f" z="%f"/>
		      <rotation unit="rad" name="pmtwinrot_%02d_%02d" x="%f" y="%f" z="%f"/>
                </physvol>""" % (i, j*pmtN+k, tempWindow_pos_V.X(), tempWindow_pos_V.Y(), tempWindow_pos_V.Z() - cerz0, i, j*pmtN+k, newrotangs[0], newrotangs[1], newrotangs[2] )


    #  Cones and shields
	    
    temp_pos_V = Pos_winstonCone_V
    temp_pos_V = pmt_rotz*temp_pos_V

    print	"""                <physvol name="physwincone_%02d">
	              <volumeref ref="logiccerwincone"/>
	              <position unit="cm" name="winconepos_%02d" x="%f" y="%f" z="%f"/>
	              <rotation unit="rad" name="winconerot_%02d" x="%f" y="%f" z="%f"/>
               </physvol>""" % (i, i, temp_pos_V.X(), temp_pos_V.Y(), temp_pos_V.Z() - cerz0, i,  newrotangs[0], newrotangs[1], newrotangs[2] )

    temp_pos_Vc = Pos_winstonCone_V + shield_offV
    temp_pos_Vc = pmt_rotz*temp_pos_Vc

    print	"""                <physvol>
		      <volumeref ref="logiccerpmtshield"/>
		      <position unit="cm" name="pmtshieldpos_%02d" x="%f" y="%f" z="%f"/>
		      <rotation unit="rad" name="pmtshieldrot_%02d" x="%f" y="%f" z="%f"/>
                </physvol>""" % (i, temp_pos_Vc.X(), temp_pos_Vc.Y(), temp_pos_Vc.Z() - cerz0, i, newrotangs[0], newrotangs[1], newrotangs[2] )

    #  Mirror
    mir1pos = mir_rotz*PosV1_temp_CM
    mir2pos = mir_rotz*PosV2_temp_CM


    print	"""                <physvol name="physcermirror1_%02d">
		      <volumeref ref="logiccermirror1"/>
		      <position unit="cm" name="mirror1pos_%02d" x="%f" y="%f" z="%f"/>
		      <rotation unit="rad" name="mirror1rot_%02d" x="%f" y="%f" z="%f"/>
                </physvol>"""  %  (i, i, mir1pos.X(), mir1pos.Y(), mir1pos.Z()- cerz0, i, 0.0, 0.0, i*2.0*pi/nsector)  # (i, mir1pos.X(), mir1pos.Y(), mir1pos.Z(), i, 0.0, 0.0, -i*2.0*pi/nsector)

    print	"""                <physvol name="physcermirror2_%02d">
		      <volumeref ref="logiccermirror2"/>
		      <position unit="cm" name="mirror2pos_%02d" x="%f" y="%f" z="%f"/>
		      <rotation unit="rad" name="mirror2rot_%02d" x="%f" y="%f" z="%f"/>
                </physvol>"""  %  (i, i, mir2pos.X(), mir2pos.Y(), mir2pos.Z()- cerz0, i, 0.0, 0.0, i*2.0*pi/nsector)  # (i, mir1pos.X(), mir1pos.Y(), mir1pos.Z(), i, 0.0, 0.0, -i*2.0*pi/nsector)

######################  Tank Windows

print """	</volume>
	<volume name="logiccerfwin">
		      <materialref ref="Aluminum"/>
		      <solidref ref="cerfrontwin"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="Gray"/>
	</volume>
	<volume name="logiccerbwin">
		      <materialref ref="Aluminum"/>
		      <solidref ref="cerbackwin"/>
	              <auxiliary auxtype="Visibility" auxvalue="true"/>
	              <auxiliary auxtype="Color" auxvalue="Gray"/>
	</volume>"""   #useful if optical photons are off


######################  Mother tank

print """
        <!---  Place volumes -->
	        <volume name="logicCerMother">
		      <materialref ref="Air"/>
		      <solidref ref="cermother"/>
	              <auxiliary auxtype="Visibility" auxvalue="false"/>
"""

print	"""                <physvol name="physcertank">
		   <volumeref ref="logiccertank"/>
		      <positionref ref="cerenkov_origin"/>
                </physvol>
                <physvol>
		   <volumeref ref="logiccerfwin"/>
	              <position name="cerenkov_fwinpos" x="0.0" y="0.0" z="%f" unit="cm"/>
                </physvol>
                <physvol>
		   <volumeref ref="logiccerbwin"/>
	              <position name="cerenkov_bwinpos" x="0.0" y="0.0" z="%f" unit="cm"/>
                </physvol>""" % (fwz-cerz0, bwz-cerz0)



print """
	        </volume>

 	     <skinsurface name="skinsrf1" surfaceproperty="MirrorSurf" >
	          <volumeref ref="logiccerwincone"/>
	     </skinsurface>
	     <skinsurface name="skinsrf2" surfaceproperty="MirrorSurf" >
	          <volumeref ref="logiccermirror1"/>
	     </skinsurface> 	
	     <skinsurface name="skinsrf3" surfaceproperty="MirrorSurf" >
	          <volumeref ref="logiccermirror2"/>
	     </skinsurface> 
	     """

#for i in range(nsector):
#   print """	         <bordersurface name="bordersrf1_%02d" surfaceproperty="MirrorSurf" >
#	          <physvolref ref="physcertank"/>
#	          <physvolref ref="physwincone_%02d"/>
#	    </bordersurface>
#            <bordersurface name="bordersrf2_%02d" surfaceproperty="MirrorSurf" >
#	          <physvolref ref="physcertank"/>
#	          <physvolref ref="physcermirror1_%02d"/>
#	    </bordersurface>
#            <bordersurface name="bordersrf3_%02d" surfaceproperty="MirrorSurf" >
#	          <physvolref ref="physcertank"/>
#	          <physvolref ref="physcermirror2_%02d"/>
#	    </bordersurface>""" % (i, i, i, i, i, i)

print """	</structure>

	<setup name="Cer" version="1.0">
	     <world ref="logicCerMother"/>
	</setup>
</gdml>"""








