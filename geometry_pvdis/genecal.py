#!/usr/bin/python
from math import sin,cos,sqrt,fabs, pi

preshowerdetno = 80000
showerdetno = 90000
ecalcycplanedetno = 70000

# ECal Paramters
# This defines a preshower and shower

zfront = 335.0
dz     = 25
zback  = zfront + 2.0*dz

#ECAL radius I used upto Wed Jan  7 13:21:40 EST 2015
#rmin = 118.0
#rmax = 261.0

#ECAL radius used in gemc
rmin = 110.0
rmax = 265.0

#6.25 cm a side gives 101.49 cm area per block
blockside = 6.25

nlayer  = 23
nsector = 30

#absorber thickness is 0.009.X_0 for shower blocks
absorbthick = 0.05
scintthick  = 0.15
gapthick = 0.024

#absorber thickness is 2.X_0 for pre-shower blocks (default value used so far 0.05137*2 : rakitha Wed Aug 20 10:03:04 EDT 2014)
#for lead X_0 = 5.612 mm
leadinglead  = 1.122
leadingscint = 2

numsides = 6
nscintlayer  = 194

deltaz_plane = 0.1
deltaz_blocker_space = 11
blockdepth = (absorbthick+scintthick+gapthick)*nscintlayer - gapthick + leadingscint
motherdepth = blockdepth + leadinglead + deltaz_plane*2 

# lab coordinate z of center of mother volume
# This needs to agree with main gdml coordinate
mother_z0 = (zfront + zback)/2 

ecalmat = "Lead"
#ecalmat = "Kryptonite"

####################################################################
def checkhexpoints( x, y, r ):
    # return true if all points lie within anulus, otherwise false
    isOK = True
    for i in range(6):
	x1 = x + r*cos(i*pi/3.0)
	y1 = y + r*sin(i*pi/3.0)

	if sqrt(x1*x1 + y1*y1) > rmax:
		isOK = False
	if sqrt(x1*x1 + y1*y1) < rmin:
		isOK = False
    return isOK


####################################################################

header = """<?xml version="1.0" encoding="UTF-8"?>
<gdml xmlns:gdml="http://cern.ch/2001/Schemas/GDML" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="schema/gdml.xsd">
    <!-- Hex Calorimeter -->

        <define>
	   <position name="ecal_origin" x="0.0" y="0.0" z="0.0" unit="cm"/>
	   <rotation name="identity"/>
	   <variable name="i0" value="1"/>"""

print header
print "        </define>"


print """        <materials>
	   <material Z="1" name="Vacuum" state="gas">
	       <T unit="K" value="2.73"/>
  	       <P unit="pascal" value="3e-18"/>
	       <D unit="g/cm3" value="1e-25"/>
	       <atom unit="g/mole" value="1.01"/>
	   </material>
	   <material name="Carbon" Z="6">
	       <D    unit="g/cm3" value="2.21"/>
	       <atom unit="g/mole" value="12.011"/>
	   </material>
	   <material name="Hydrogen" Z="1">
	       <D    unit="g/cm3" value="0.067"/>
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

	   <material Z="82" name="Lead" state="solid">
	       <D unit="g/cm3" value="11.350"/>
	       <atom unit="g/mole" value="207.2"/>
	   </material>
	   <material Z="82" name="Kryptonite" state="solid">
	       <D unit="g/cm3" value="11.350"/>
	       <atom unit="g/mole" value="207.2"/>
	   </material>

	   <material name="Scint" state="gas">
	       <D unit="g/cm3" value="1.032"/>
	       <composite n="10" ref="Hydrogen"/>
	       <composite n="9" ref="Carbon"/>
	   </material>

	   <material name="Air" state="gas">
	       <D value="0.00129" />
	       <fraction n="0.7" ref="Nitrogen" />
	       <fraction n="0.3" ref="Oxygen" />
	   </material>

        </materials>
	
	<solids>"""

print """	      <tube name="ecalmother" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%3.4f" rmax="%3.4f" z="%3.4f"/>"""% (rmin, rmax, motherdepth+deltaz_blocker_space)
print """	      <tube name="ecalcycdet_solid_1" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="0" rmax="%3.4f" z="%3.4f"/>"""% (rmax-0.1, motherdepth-0.05)
print """	      <tube name="ecalcycdet_solid_2" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="0" rmax="%3.4f" z="%3.4f"/>"""% (rmax-0.2, motherdepth-0.1)
print """	      <tube name="ecalcycdet_solid_3" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="0" rmax="%3.4f" z="%3.4f"/>"""% (rmin, motherdepth)

#create 30 photon blocker solids
for i in range(30):
    print """	      <tube name="ecalphotblock_solid_%d" aunit="deg" startphi="%f" deltaphi="2.5" lunit="cm" rmin="110" rmax="200" z="5"/>"""% (i+1, (i*12+2.2))


print """  <subtraction name ="ecalcycdet_solid_4">
      <first ref="ecalcycdet_solid_1"/>
      <second ref="ecalcycdet_solid_2"/>
      <positionref ref="ecal_origin"/>
      <rotationref ref="identity"/>
  </subtraction> """ 

print """  <subtraction name ="ecalcycdet_solid">
      <first ref="ecalcycdet_solid_4"/>
      <second ref="ecalcycdet_solid_3"/>
      <positionref ref="ecal_origin"/>
      <rotationref ref="identity"/>
  </subtraction> """ 

print """	      <polyhedra name="ecalblock" aunit="deg" startphi="0" deltaphi="360" lunit="cm" numsides="%d" >
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
	      </polyhedra>""" % (numsides,  blockside*sqrt(3.0)/2, -blockdepth/2, blockside*sqrt(3.0)/2,  blockdepth/2 )

print """	      <tube name="ecalleadinglead" aunit="deg" startphi="0" deltaphi="360" lunit="cm" rmin="%3.4f" rmax="%3.4f" z="%3.4f"/>"""% (rmin, rmax, leadinglead)

print """	      <polyhedra name="ecalleadscint" aunit="deg" startphi="0" deltaphi="360" lunit="cm" numsides="%d" >
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
	      </polyhedra>""" % (numsides,  blockside*sqrt(3.0)/2, -leadingscint/2, blockside*sqrt(3.0)/2,  leadingscint/2)

print """	      <polyhedra name="ecalgap" aunit="deg" startphi="0" deltaphi="360" lunit="cm" numsides="%d" >
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
	      </polyhedra>""" % (numsides,  blockside*sqrt(3.0)/2, -gapthick/2, blockside*sqrt(3.0)/2,  gapthick/2)
print """	      <polyhedra name="ecalblockscint" aunit="deg" startphi="0" deltaphi="360" lunit="cm" numsides="%d" >
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
   	          <zplane rmin="0" rmax="%3.4f" z="%3.4f"/>
	      </polyhedra>""" % (numsides,  blockside*sqrt(3.0)/2, -scintthick/2, blockside*sqrt(3.0)/2,  scintthick/2)

print """        </solids>

	<structure>
"""

##create 30 photon blocker logic volumes
for i in range(30):
    print """	         <volume name="ecalphotblock_logic_%d">
		      <materialref ref="Lead"/> 
		      <solidref ref="ecalphotblock_solid_%d"/>
		      <auxiliary auxtype="Visibility" auxvalue="true"/>
	        </volume>""" % (i+1,i+1 )  
    

print """	         <volume name="ecalcycdet_logic">
		      <materialref ref="Vacuum"/> 
		      <solidref ref="ecalcycdet_solid"/>
		      <auxiliary auxtype="Visibility" auxvalue="true"/>
		       <auxiliary auxtype="SensDet" auxvalue="planeDet"/>
		      <auxiliary auxtype="DetNo" auxvalue="%d"/>
	        </volume>""" % (ecalcycplanedetno)  # assign 70000 to plane cyclindrical detector

print """	         <volume name="logicecalleadinglead">
		      <materialref ref="%s"/> 
		      <solidref ref="ecalleadinglead"/>
		      <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Gray"/>
		       <auxiliary auxtype="SensDet" auxvalue="planeDet"/>
		      <auxiliary auxtype="DetNo" auxvalue="%d"/>
	        </volume>""" % (ecalmat, showerdetno+9999)  # assign 99999 to leading lead (useful for kryptonite runs)
print """	         <volume name="logicecalleadscint">
		      <materialref ref ="Scint"/> 
		      <solidref ref ="ecalleadscint"/>
		      <auxiliary auxtype="Visibility" auxvalue="true"/>
		      <auxiliary auxtype="Color" auxvalue="Yellow"/>
		      <auxiliary auxtype="SensDet" auxvalue="Cal"/>
		      <auxiliary auxtype="DetNo" auxvalue="%d"/>
	        </volume>""" % preshowerdetno

print """	         <volume name="logicecalgap">
		      <materialref ref ="Air"/> 
		      <solidref ref ="ecalgap"/>
		      <auxiliary auxtype="Visibility" auxvalue="false"/>
	        </volume>"""

print """	         <volume name="logicecalblockscint">
		      <materialref ref ="Scint"/> 
		      <solidref ref ="ecalblockscint"/>
		      <auxiliary auxtype="Visibility" auxvalue="false"/>
<!--		      <auxiliary auxtype="Color" auxvalue="Yellow"/> -->
		      <auxiliary auxtype="SensDet" auxvalue="Cal"/>
		      <auxiliary auxtype="DetNo" auxvalue="%d"/>
	        </volume>""" % showerdetno

# Now build blocks		
# Preshower scint + mainshower layers

print """	         <volume name="logicecalblock">
		      <materialref ref ="%s"/> 
		      <solidref ref ="ecalblock"/>
		      <auxiliary auxtype="Visibility" auxvalue="True"/>
		      <auxiliary auxtype="Color" auxvalue="Gray"/> 
		      """ % (ecalmat)
print """	              <physvol>
			     <volumeref ref="logicecalleadscint" />
			     <position name="ecalleadscintpos" unit="cm" x="0.0" y="0.0" z="%3.4f"/>
                        </physvol>""" % (-blockdepth/2 + leadingscint/2)

for i in range(nscintlayer):
    thiszoff = -blockdepth/2 + leadingscint + (i+1)*absorbthick + i*(scintthick + gapthick)
    print """	              <physvol>
			     <volumeref ref="logicecalblockscint" />
			     <position name="ecalblockscintpos_%d" unit="cm" x="0.0" y="0.0" z="%3.4f"/>
                        </physvol>""" %  (i, 0.5*scintthick + thiszoff)
    print """	              <physvol>
			     <volumeref ref="logicecalgap" />
			     <position name="ecalgappos_%d" unit="cm" x="0.0" y="0.0" z="%3.4f"/>
                        </physvol>""" %  (i,0.5*gapthick + scintthick + thiszoff)

print """	        </volume>""" # logicecalblock


print """
        <!---  Place volumes -->
	        <volume name="logicEcalMother">
		      <materialref ref="Vacuum"/>
		      <solidref ref="ecalmother"/>
	              <auxiliary auxtype="Visibility" auxvalue="false"/>"""

for i in range(30):
    print """	      <physvol>
                      <volumeref ref="ecalphotblock_logic_%d"/>
		      <position name="ecalphotblock_pos_%d" unit="cm" x="0.0" y ="0.0" z="-26.0"/>
	        </physvol>""" % (i+1, i+1 ) 

print """	      <physvol>
                      <volumeref ref="ecalcycdet_logic"/>
		      <position name="ecalleadingleadpos" unit="cm" x="0.0" y ="0.0" z="0"/>
	        </physvol>""" 

print """	      <physvol>
                      <volumeref ref="logicecalleadinglead"/>
		      <position name="ecalleadingleadpos" unit="cm" x="0.0" y ="0.0" z="%3.4f"/>
	        </physvol>""" % (-motherdepth/2 + deltaz_plane/2 + leadinglead/2)

# number of x and y steps to consider
xspace = blockside*3
yspace = blockside*sqrt(3.0)


nx = int(2.0*rmax/xspace) + 1
ny = int(2.0*rmax/yspace) + 1
for i in range(nx):
    for j in range(ny):
	thisx = -rmax + i*xspace
	thisy = -rmax + j*yspace

	if checkhexpoints( thisx, thisy, blockside ):
	    print """             <physvol>
		  <volumeref ref="logicecalblock"/>
		  <position name="ecalblockpos_%d_%d_%d" unit="cm" x="%3.2f" y ="%3.2f" z="%3.2f"/>
	    </physvol>""" % (i, j, 0, thisx, thisy, motherdepth/2-blockdepth/2)
#add 0.5 xspace and 0.5 yspace offsets and repeat above
	thisx = -rmax + i*xspace+ blockside*1.5
        thisy = -rmax + j*yspace + blockside*sqrt(3.0)/2

	if checkhexpoints( thisx, thisy, blockside ):
	    print """             <physvol>
		  <volumeref ref="logicecalblock"/>
		  <position  name="ecalblockpos_%d_%d_%d " unit="cm" x="%3.2f" y ="%3.2f" z="%3.2f"/>
	    </physvol>""" % (i, j, 1, thisx, thisy, motherdepth/2-blockdepth/2)

print """      </volume>
	</structure>

	<setup name="Ecal" version="1.0">
	     <world ref="logicEcalMother"/>
	</setup>
</gdml>"""








