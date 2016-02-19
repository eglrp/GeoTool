#include "HJ1ACorrect.h"





HJ1ACorrect::HJ1ACorrect()
{

}



int HJ1ACorrect::imagingRay(const ossimDpt& image_point,
			   ossimEcefRay& image_ray) 
{
	bool runtime_dbflag = 0;
	NEWMAT::Matrix satToOrbit,camToSat,pianzhi;

	ossimDpt iPt = image_point;
	
	//
	// 1. Establish time of line imaging :  
	//
	double t_line = getTimeLine(iPt.line);

	//
	// 2. Interpolate ephemeris position and velocity (in ECF):
	//
	ossimEcefPoint  tempEcefPoint,tempEciPoint,tempEciPoint1;
	ossimEcefPoint  P_ecf,P_eci,P_eci1;
	getPositionEcf(t_line, P_ecf);
	getVelocityEcf(t_line, tempEcefPoint);

	double rot = 7.292115856e-05;
//	tempEcefPoint.x() -= rot*P_ecf.y();
//	tempEcefPoint.y() += rot*P_ecf.x();

	double V[3],VV[3];
	V[0] = tempEcefPoint.x() - rot*P_ecf.y();
	V[1] = tempEcefPoint.y() + rot*P_ecf.x();
	V[2] = tempEcefPoint.z();
/*
	CivilTime utc(short(2004),short(1),short(1),short(0),short(0),0.0);
	CommonTime UTC(utc);	
	UTC.addSeconds(t_line);
	//	 CivilTime utc1(UTC);
	UTC.setTimeSystem(7);

	Vector<double> ecefPosVel(6,0.0),j2kPosVel(6,0.0),j2kPos(3,0.0),ecefPos(3,0.0),j2kVel(3,0.0),ecefVel(3,0.0);


	ecefPosVel[0] = P_ecf.x();
	ecefPosVel[1] = P_ecf.y();
	ecefPosVel[2] = P_ecf.z();
	ecefPosVel[3] = tempEcefPoint.x();
	ecefPosVel[4] = tempEcefPoint.y();
	ecefPosVel[5] = tempEcefPoint.z();

	j2kPosVel = ECEFPosVelToJ2k(UTC,ecefPosVel);
	P_eci.x() = j2kPosVel[0];
	P_eci.y() = j2kPosVel[1];
	P_eci.z() = j2kPosVel[2];
	VV[0] = j2kPosVel[3];
	VV[1] = j2kPosVel[4];
	VV[2] = j2kPosVel[5];
*/
	ossimEcefVector V_ecf(V[0],
		V[1],
		V[2]);

/*	ossimEcefVector V_ecf(tempEcefPoint.x(),
		tempEcefPoint.y(),
		tempEcefPoint.z());
*/
	//
	// 3. Establish the look direction in Vehicle LSR space (S_sat).
	//    ANGLES IN RADIANS
	//
	ossim_float64 Psi_x;
	getPixelLookAngleX(iPt.samp, Psi_x);
	ossim_float64 Psi_y;
	getPixelLookAngleY(iPt.samp, Psi_y);



//	ossimColumnVector3d u_sat (-tan(Psi_y), tan(Psi_x), -(1.0 ));

	ossimColumnVector3d u_cam (-tan(Psi_y), tan(Psi_x), -(1.0 ));

//	ossimColumnVector3d u_cam (-(Psi_y), 0, -(CCD1_FOCAL_LENGTH ));
//	ossimColumnVector3d u_cam (0, -(Psi_y), -(CCD1_FOCAL_LENGTH ));

//	u_cam = u_cam.unit();
	//°²×°¾ØÕó
	computeCamToSatRotation(camToSat);

//	computePianzhiRotation(pianzhi);

	ossimColumnVector3d u_sat = ((camToSat*u_cam)).unit();
	

	//
	// 4. Transform vehicle LSR space look direction vector to orbital LSR space
	//    (S_orb):
	//
	computeSatToOrbRotation(satToOrbit, t_line);
//	computeSatToOrbRotationQ(satToOrbit, t_line);

	ossimColumnVector3d u_orb = (satToOrbit*u_sat).unit();


	//
	// 5. Transform orbital LSR space look direction vector to ECF.
	//
	//   a. S_orb space Z-axis (Z_orb) is || to the ECF radial vector (P_ecf),
	//   b. X_orb axis is computed as cross-product between velocity and radial,
	//   c. Y_orb completes the orthogonal S_orb coordinate system.
	//
/*
	ossimColumnVector3d Z_orb1 (P_ecf.x(),
		P_ecf.y(),
		P_ecf.z());
	Z_orb1 = Z_orb1.unit();
*/
/*
	ossimColumnVector3d Z_orb1 (P_ecf.x(),
		P_ecf.y(),
		P_ecf.z());
	Z_orb1 = Z_orb1.unit();

	ossimColumnVector3d X_orb1 = (ossimColumnVector3d(V_ecf.x(),
		V_ecf.y(),
		V_ecf.z()).cross(Z_orb1)).unit();
	ossimColumnVector3d Y_orb1 = Z_orb1.cross(X_orb1);
//	Z_orb1 = Z_orb1.unit();

	NEWMAT::Matrix orbToEcfRotation = NEWMAT::Matrix(3, 3);
	orbToEcfRotation << X_orb1[0] << Y_orb1[0] << Z_orb1[0]
	<< X_orb1[1] << Y_orb1[1] << Z_orb1[1]
	<< X_orb1[2] << Y_orb1[2] << Z_orb1[2];


	ossimColumnVector3d u_ecf1  = (orbToEcfRotation*u_orb);

	//
	// Establish the imaging ray given direction and origin:
	//
	image_ray = ossimEcefRay(P_ecf, ossimEcefVector(u_ecf1[0], u_ecf1[1], u_ecf1[2]));
*/
	ossimColumnVector3d Z_orb (P_ecf.x(),
		P_ecf.y(),
		P_ecf.z());
	Z_orb = Z_orb.unit();

	ossimColumnVector3d X_orb = ossimColumnVector3d(V_ecf.x(),
		V_ecf.y(),
		V_ecf.z()).cross(Z_orb).unit();
	ossimColumnVector3d Y_orb = Z_orb.cross(X_orb);

	NEWMAT::Matrix orbToEcfRotation = NEWMAT::Matrix(3, 3);
	orbToEcfRotation << X_orb[0] << Y_orb[0] << Z_orb[0]
	<< X_orb[1] << Y_orb[1] << Z_orb[1]
	<< X_orb[2] << Y_orb[2] << Z_orb[2];


	ossimColumnVector3d u_ecf  = (orbToEcfRotation*u_orb);

	//
	// Establish the imaging ray given direction and origin:
	//
	image_ray = ossimEcefRay(P_ecf, ossimEcefVector(u_ecf[0], u_ecf[1], u_ecf[2]));

	return 0;
}


HJ1ACorrect::~HJ1ACorrect()
{

}

