/***
 * @file InverseKinematic.cpp
 * @brief IK implementation for the Novint Falcon based on paper by R.E. Stamper (http://libnifalcon.wiki.sourceforge.net/space/showimage/PhD_97-4.pdf)
 * @author Kevin Ouellet (kouellet@users.sourceforge.net)
 * @version $Id$
 * @copyright (c) 2007-2008 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * $HeadURL$
 * 
 * Project info at http://libnifalcon.sourceforge.net/ 
 *
 */
#include "falcon/kinematic/stamper/InverseKinematic.h"

using namespace gmtl;
using namespace gmtl::Math;

namespace libnifalcon
{
	namespace StamperKinematicImpl
	{

		Angle InverseKinematic::calculate(gmtl::Point3f position)
		{
			Angle angle;
			calulateArm(ARM_1, position, &angle);
			calulateArm(ARM_2, position, &angle);
			calulateArm(ARM_3, position, &angle);
			return angle;
		}

		void InverseKinematic::calulateArm(arm_id arm, Point3f position, Angle *angle)
		{
			float pu = -r+Math::cos(phy[arm])*position[X]+Math::sin(phy[arm])*position[Y];
			float pv = -(s-Math::cos(phy[arm])*position[Y]+Math::sin(phy[arm])*position[X]);
			float pw = position[Z];
			
			float theta3 = Math::aCos((pv+f)/b);
	
			float l0 = pow(pw,2) + pow(pu,2) + 2*c*pu - 2*a*pu + pow(a,2) + pow(c,2) - pow(d,2) - pow(e,2) 
				- pow(b,2)*pow(Math::sin(theta3),2) - 2*b*e*Math::sin(theta3) - 2*b*d*Math::sin(theta3) - 2*d*e - 2*a*c;
	
			float l1 = -4*a*pw;
	
			float l2 = pow(pw,2) + pow(pu,2) + 2*c*pu + 2*a*pu + pow(a,2) + pow(c,2) - pow(d,2) - pow(e,2) 
				- pow(b,2)*pow(Math::sin(theta3),2) - 2*b*e*Math::sin(theta3) - 2*b*d*Math::sin(theta3) - 2*d*e + 2*a*c;
	
			float t = -(sqrt(pow(l1,2)-4*l2*l0)+l1)/(2*l2);
		
			float theta1 = Math::aTan(t)*2;
	
			float theta2 = Math::aCos(-(a*Math::cos(theta1)-c-pu)/(b*Math::sin(theta3)+d+e));
	
			angle->theta1[arm] = theta1;
			angle->theta2[arm] = theta2;
			angle->theta3[arm] = theta3;
		}
	}
}
