/*
 * goldenGate.c
 *
 *  Created on: Aug 29, 2023
 *      Author: ashlo
 */

// mode=1 ICQ
// mode=2 OCC
// function hardcoding
// XDL - 8
// XBL - 1 (debatable)
// WBL - 4
#include<custom_h.h>
#include <xtime_l.h>
// below is from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\024_InternalMode\Sno_2_AtSpeed_1XInternalMode_analyse.mlx
u8 ADC_outs_total[72]       ={2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89};
// Allocate memory for the Mapped_bins array
float ADC_bins[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
//uint16_t IMC_Out_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
u16 Addertree_dec_ADC[XDL_HW][WBL_HW];
float central_ref[XDL_HW];
float MAc_ADC[XDL_HW][XBL_HW][WBL_HW][WDL_HW];

// SLOPE AND OFFSET SETTINGS #1 below 2 are from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\023_ADC_HybridMode\INLDNL_Analysis\getINL_TB.m
//float m_ADC[92]= {0,0,1.025107,1.032128,1.025489,1.032700,1.024191,1.031287,1.026023,1.030712,1.025060,1.031913,1.026347,1.032243,1.025374,1.031215,1.024484,1.031601,1.025773,1.050062,0.992501,0.997822,0.989477,0.998443,0.994093,0.999253,0.991273,0.999457,1.024376,1.031077,1.024173,1.030725,1.024153,1.029870,1.022494,1.028598,1.023219,1.030670,1.022650,1.029159,1.020680,1.028810,1.022183,1.028108,1.021022,1.027716,1.024668,1.027907,1.021991,1.028341,1.026507,1.026733,1.020768,1.026706,1.021465,1.028195,1.020321,1.026459,1.019143,1.027220,1.019137,1.028125,1.019141,1.025985,0.993579,0.998046,0.992166,0.999723,0.992322,0.998132,0.994262,1.000540,1.067152,1.026526,1.016310,1.021558,1.017316,1.022121,1.016461,1.022199,1.031372,1.020864,1.012468,1.020531,1.010041,1.016970,1.010002,1.018126,1.010373,1.014538,0,0};
//float c_ADC[92]={0,0,-6.034851,-4.905083,-5.885475,-5.286806,-5.308894,-4.890551,-6.363450,-5.376364,-5.763179,-4.893069,-6.139452,-5.150959,-5.879969,-4.617934,-5.278545,-4.773973,-5.529030,-6.235813,-3.082974,-2.640330,-1.871929,-1.599959,-3.344192,-2.193959,-2.285426,-1.847942,-5.596451,-5.016215,-5.919217,-5.001976,-5.701873,-3.989588,-4.897534,-4.148247,-5.411687,-5.865431,-5.221248,-3.406162,-4.005877,-3.630097,-5.695843,-4.193620,-5.182105,-4.163274,-6.455421,-3.625954,-4.979580,-4.848085,-5.482722,-4.242137,-4.847774,-4.279084,-4.871782,-5.155615,-4.953736,-4.173928,-4.422278,-3.879749,-5.071174,-4.117119,-4.428910,-4.930677,-2.848711,-1.177514,-2.556141,-2.677568,-1.625951,-1.652792,-3.183424,-2.203051,-9.715866,-4.946203,-4.612488,-3.283906,-5.314438,-3.935548,-5.279199,-4.144894,-6.359056,-3.936474,-4.067334,-4.104750,-2.575834,-2.834948,-4.124738,-3.861806,-4.932272,-2.957848,0,0};

// SLOPE AND OFFSET SETTINGS #2 below 2 are from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\031_ADC_Ashwin\ASHWIN_ADDED_CODES\HYBRID_SPEED_SETTING_70ns\ADC_INL_code.m
//float m_ADC[92]={0.000000,0.000000,1.027407,1.033930,1.027270,1.034654,1.026449,1.034117,1.028079,1.034482,1.027032,1.033867,1.027860,1.034549,1.027409,1.034430,1.026998,1.033797,1.027212,1.050062,0.992501,0.997822,0.989477,0.998443,0.994093,0.999253,0.991273,0.999457,1.025486,1.032384,1.026259,1.033176,1.025488,1.032464,1.024260,1.031455,1.025339,1.032034,1.024454,1.032061,1.023357,1.031416,1.024027,1.031125,1.023139,1.030578,1.024810,1.029968,1.022931,1.029985,1.028827,1.029818,1.021906,1.029596,1.022304,1.029366,1.021342,1.028624,1.020567,1.028999,1.020723,1.029637,1.019582,1.026469,0.993579,0.998046,0.992166,0.999723,0.992322,0.998132,0.994262,1.000540,1.067152,1.025905,1.017056,1.024859,1.016561,1.023318,1.015509,1.022481,1.032886,1.021814,1.012853,1.020331,1.011258,1.019301,1.010247,1.016988,1.009553,1.016754,0.000000,0.000000} ;
//float c_ADC[92]={0.000000,0.000000,-2.801388,-2.245754,-2.703078,-2.434610,-2.424590,-2.280900,-3.004275,-2.552162,-2.663159,-2.266627,-2.821116,-2.404149,-2.718264,-2.155696,-2.443941,-2.201416,-2.534900,-6.235813,-3.082974,-2.640330,-1.871929,-1.599959,-3.344192,-2.193959,-2.285426,-1.847942,-2.552930,-2.300006,-2.745606,-2.340870,-2.618778,-1.847589,-2.237552,-1.933869,-2.492240,-2.742745,-2.393339,-1.568299,-1.848879,-1.729850,-2.654528,-1.980121,-2.429594,-1.960059,-2.969185,-1.678133,-2.264920,-2.233153,-2.576973,-2.003812,-2.195255,-2.011047,-2.230258,-2.402283,-2.250682,-1.954053,-2.013936,-1.776246,-2.340786,-1.883485,-1.992938,-2.285988,-2.848711,-1.177514,-2.556141,-2.677568,-1.625951,-1.652792,-3.183424,-2.203051,-9.715866,-2.244646,-2.084428,-1.565467,-2.375720,-1.772372,-2.360643,-1.893890,-2.990030,-1.778713,-1.791070,-1.857459,-1.115131,-1.281088,-1.809867,-1.697298,-2.179908,-1.329769,0.000000,0.000000} ;

// SLOPE AND OFFSET SETTINGS #3
// below 2 are from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\031_ADC_Ashwin\InternalSynthetic\1_XFER\ADC_7b_SO_corrn.txt
//  for timing from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\031_ADC_Ashwin\InternalSynthetic\TIMING91\try_6b\get_GLOBALTIMER_scanInPattern.m
float m_ADC[92]={0.000000,0.000000,1.005272,1.008631,1.004831,1.009549,1.004664,1.009249,1.007084,1.010784,1.006228,1.010702,1.006856,1.010984,1.006072,1.009978,1.003955,1.009952,1.006061,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.006646,1.011021,1.008005,1.011825,1.007704,1.011216,1.005997,1.010860,1.007148,1.013384,1.005920,1.012500,1.004750,1.010120,1.008923,1.011031,1.007132,1.010172,1.011900,1.010633,1.007458,1.013026,0.000000,1.012945,1.006212,1.012450,1.006205,1.012930,1.006180,1.013026,1.005487,1.012321,1.009142,1.027651,1.006856,1.011786,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.014799,1.007274,1.010258,1.010593,1.012725,1.010294,1.012249,0.000000,1.012647,1.006390,1.012293,1.004529,1.011911,1.008481,1.012429,1.009491,1.009589,0.000000,0.000000};
float c_ADC[92]={0.000000,0.000000,-2.323510,-1.175869,-2.187346,-1.698705,-1.731558,-1.358816,-2.807146,-2.028563,-2.056155,-1.330853,-2.482838,-1.685440,-2.202163,-1.052063,-1.538953,-1.148036,-1.864455,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-1.944871,-1.483395,-2.427981,-1.568665,-2.162525,-0.563947,-1.446951,-0.774392,-1.911846,-2.480072,-1.799730,-0.006780,-0.614746,-0.288333,-2.397387,-0.964788,-1.859291,-0.936688,-3.263022,-0.459196,-1.677354,-1.548729,0.000000,-1.175672,-1.566053,-1.207641,-1.623669,-2.115933,-1.683402,-1.206188,-1.210556,-0.706659,-2.146109,-1.381530,-1.351443,-2.032798,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,-2.246354,-1.798585,-0.518583,-2.592334,-1.221542,-2.783373,-1.583551,0.000000,-1.416679,-1.571575,-1.789076,-0.130480,-0.607583,-1.917333,-1.649010,-2.799636,-0.867884,0.000000,0.000000};



// OLD VALUES
//float m1R[72]={16762.800000,16848.200000,16756.700000,16976.300000,16982.400000,16793.300000,16768.900000,17043.400000,16976.300000,17000.700000,17000.700000,16823.800000,16775.000000,17025.100000,16982.400000,16799.400000,16988.500000,16939.700000,17031.200000,17116.600000,17049.500000,16836.000000,16793.300000,16927.500000,17012.900000,16750.600000,16744.500000,16756.700000,16768.900000,16805.500000,16915.300000,16732.300000,16823.800000,16805.500000,16890.900000,17037.300000,16799.400000,16829.900000,16732.300000,16848.200000,16732.300000,16823.800000,16994.600000,16982.400000,16756.700000,16903.100000,16811.600000,16720.100000,17025.100000,16805.500000,16976.300000,17012.900000,16805.500000,16762.800000,16927.500000,16805.500000,16817.700000,16805.500000,16811.600000,16945.800000,17025.100000,17049.500000,16836.000000,16799.400000,16689.600000,16988.500000,16842.100000,16805.500000,16756.700000,16848.200000,16970.200000,16927.500000};
//float m2[72]={1.065207,1.077715,1.068520,1.067020,1.079879,1.078069,1.067397,1.065857,1.066726,1.068980,1.067889,1.068341,1.080335,1.064980,1.076275,1.068050,1.076324,1.066160,1.077915,1.069939,1.078121,1.070577,1.070158,1.077265,1.075982,1.081535,1.079456,1.068495,1.067135,1.071194,1.068422,1.079664,1.079569,1.078705,1.070071,1.081164,1.079845,1.070440,1.068069,1.080996,1.079384,1.078709,1.079597,1.083275,1.068069,1.081029,1.081224,1.069721,1.080939,1.079244,1.080271,1.081722,1.080099,1.069945,1.072056,1.071014,1.069699,1.069374,1.079250,1.080099,1.083582,1.070049,1.069766,1.071975,1.069421,1.068449,1.067249,1.067506,1.067897,1.069190,1.079479,1.069126 };

// From D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\028_SystemMAC_DryRuns\CURRENTMIRRORCalibPattern\OUT_m1R_m2.txt
//float m2[72]={9.905075e-01,1.004051e+00,1.006072e+00,1.007702e+00,9.963650e-01,9.954237e-01,1.005740e+00,1.004358e+00,9.951638e-01,1.005147e+00,1.005230e+00,1.004242e+00,9.957325e-01,1.008023e+00,1.004486e+00,1.005183e+00,1.004540e+00,1.003749e+00,9.926400e-01,1.006001e+00,1.006879e+00,1.007251e+00,1.006494e+00,1.006549e+00,1.004736e+00,9.953325e-01,9.959387e-01,1.009842e+00,1.003496e+00,1.006701e+00,1.005139e+00,1.007471e+00,1.007449e+00,1.007749e+00,1.006356e+00,1.009576e+00,1.006749e+00,1.009795e+00,9.957350e-01,9.950537e-01,1.008472e+00,1.006977e+00,1.007274e+00,1.010709e+00,9.977712e-01,1.007605e+00,9.958625e-01,9.971312e-01,1.009086e+00,9.937512e-01,9.978738e-01,9.950700e-01,1.008000e+00,1.005079e+00,1.007355e+00,9.992163e-01,1.009735e+00,1.006119e+00,9.935412e-01,1.008091e+00,9.990012e-01,1.006797e+00,9.975650e-01,9.994850e-01,1.007245e+00,1.004920e+00,9.952962e-01,1.006035e+00,9.974787e-01,9.952412e-01,9.932212e-01,1.004687e+00, };
float m1R_1X[72]={8.372250e+03,8.277700e+03,8.396650e+03,8.289900e+03,8.387500e+03,8.384450e+03,8.311250e+03,8.225850e+03,8.360050e+03,8.247200e+03,8.299050e+03,8.393600e+03,8.405800e+03,8.408850e+03,8.274650e+03,8.378350e+03,8.271600e+03,8.277700e+03,8.265500e+03,8.424100e+03,8.329550e+03,8.268550e+03,8.256350e+03,8.308200e+03,8.292950e+03,8.289900e+03,8.262450e+03,8.299050e+03,8.289900e+03,8.265500e+03,8.311250e+03,8.268550e+03,8.390550e+03,8.399700e+03,8.268550e+03,8.390550e+03,8.399700e+03,8.384450e+03,8.384450e+03,8.390550e+03,8.414950e+03,8.314300e+03,8.292950e+03,8.299050e+03,8.378350e+03,8.299050e+03,8.408850e+03,8.347850e+03,8.302100e+03,8.308200e+03,8.268550e+03,8.283800e+03,8.399700e+03,8.289900e+03,8.198400e+03,8.405800e+03,8.311250e+03,8.262450e+03,8.296000e+03,8.292950e+03,8.399700e+03,8.384450e+03,8.250250e+03,8.299050e+03,8.299050e+03,8.402750e+03,8.286850e+03,8.299050e+03,8.408850e+03,8.396650e+03,8.314300e+03,8.436300e+03 };
float m1R_2X[72]={1.676280e+04,1.684820e+04,1.675670e+04,1.697630e+04,1.698240e+04,1.679330e+04,1.676890e+04,1.704340e+04,1.697630e+04,1.700070e+04,1.700070e+04,1.682380e+04,16775,1.702510e+04,1.698240e+04,1.679940e+04,1.698850e+04,1.693970e+04,1.703120e+04,1.711660e+04,1.704950e+04,1.683600e+04,1.679330e+04,1.692750e+04,1.701290e+04,1.675060e+04,1.674450e+04,1.675670e+04,1.676890e+04,1.680550e+04,1.691530e+04,1.673230e+04,1.682380e+04,1.680550e+04,1.689090e+04,1.703730e+04,1.679940e+04,1.682990e+04,1.673230e+04,1.684820e+04,1.673230e+04,1.682380e+04,1.699460e+04,1.698240e+04,1.675670e+04,1.690310e+04,1.681160e+04,1.672010e+04,1.702510e+04,1.680550e+04,1.697630e+04,1.701290e+04,1.680550e+04,1.676280e+04,1.692750e+04,1.680550e+04,1.681770e+04,1.680550e+04,1.681160e+04,1.694580e+04,1.702510e+04,1.704950e+04,16836,1.679940e+04,1.668960e+04,1.698850e+04,1.684210e+04,1.680550e+04,1.675670e+04,1.684820e+04,1.697020e+04,1.692750e+04 };
//float m1R_4X[72]={3.406240e+04,3.354651e+04,3.369989e+04,3.397874e+04,3.364411e+04,3.414606e+04,3.363017e+04,3.369989e+04,3.407634e+04,3.364411e+04,3.418789e+04,3.363017e+04,3.403451e+04,3.389509e+04,3.342103e+04,3.396480e+04,3.406240e+04,3.393691e+04,3.356046e+04,3.356046e+04,3.360229e+04,3.372777e+04,3.360229e+04,34160,3.364411e+04,3.414606e+04,3.395086e+04,3.376960e+04,3.361623e+04,3.396480e+04,3.365806e+04,3.409029e+04,3.360229e+04,3.363017e+04,3.406240e+04,3.361623e+04,3.402057e+04,3.413211e+04,3.418789e+04,34160,3.388114e+04,3.365806e+04,3.403451e+04,3.400663e+04,3.360229e+04,3.360229e+04,3.406240e+04,3.410423e+04,3.400663e+04,3.407634e+04,3.365806e+04,3.365806e+04,3.357440e+04,3.364411e+04,3.350469e+04,3.369989e+04,3.360229e+04,3.356046e+04,3.374171e+04,3.413211e+04,3.360229e+04,3.356046e+04,3.378354e+04,3.375566e+04,33672,3.356046e+04,3.351863e+04,3.383931e+04,3.411817e+04,3.399269e+04,3.393691e+04,3.416000e+04 };

// below is from D:\Xilinx\Vivado_2018_Projs\IMC_CHIP_2023\PCB_v3\MATLAB_CONTROL\Control_Codes\Testing\034_Alphas_Ashwin\1_XFER\MODELLING_RESULTS.txt
u8 map_to_72[WDL_HW]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,41,42,43,44,45,46,47,48,49,50,51,52,53,55,56,57,58,59,60,61,63,64,65,66,67,68,69,70,71};
float m1R_4X[72]={31199.312609,31849.155651,31960.112360,32474.500592,32190.199640,32037.070965,32090.041013,32053.355958,32834.128418,32622.612467,32571.867527,32000.556758,32815.839866,32606.103766,32239.381896,32683.053257,32791.185651,32458.892698,31817.828959,32267.383106,32316.775466,32066.963608,32370.759812,32599.199099,32318.547344,32954.187735,32610.990709,32017.292638,32231.008138,32673.575694,32760.053921,32661.958940,32342.333092,32493.918368,32639.139839,32159.088926,32275.197467,31961.950015,32063.777935,31982.052007,32688.386402,32001.553209,32191.535638,32425.052910,32347.226513,31964.486036,32120.234967,32068.992192,32404.292873,32489.912037,31878.783414,32053.491870,31837.663741,32125.064285,31802.424521,32179.247353,31991.681880,32200.387209,32409.634566,32130.148796,32024.901550,31853.952841,32843.937125,31765.785128,32421.875962,32509.296130,31449.894289,31913.061461,32252.695116,32608.880663,32748.491844,32843.971889};
float m2[72]={0.990507,1.004051,1.006072,1.007702,0.996365,0.995424,1.005740,1.004358,0.995164,1.005147,1.005230,1.004242,0.995732,1.008023,1.004486,1.005183,1.004540,1.003749,0.992640,1.006001,1.006879,1.007251,1.006494,1.006549,1.004736,0.995332,0.995939,1.009842,1.003496,1.006701,1.005139,1.007471,1.007449,1.007749,1.006356,1.009576,1.006749,1.009795,0.995735,0.995054,1.008472,1.006977,1.007274,1.010709,0.997771,1.007605,0.995862,0.997131,1.009086,0.993751,0.997874,0.995070,1.008000,1.005079,1.007355,0.999216,1.009735,1.006119,0.993541,1.008091,0.999001,1.006797,0.997565,0.999485,1.007245,1.004920,0.995296,1.006035,0.997479,0.995241,0.993221,1.004687};
float LT[72]={14.794043,18.458780,19.662272,19.332718,19.881641,18.508995,18.785064,18.682286,19.021280,19.045263,19.570159,19.557920,19.273116,18.923651,18.965386,19.210680,18.664847,19.050497,15.888023,19.317834,19.579266,19.454746,19.598456,18.914394,18.631901,19.422427,19.233335,19.281018,18.873799,19.366229,19.037108,18.808524,19.303643,19.382917,19.675099,19.924357,15.880490,16.929337,15.098382,15.645198,16.425161,15.947561,16.052276,16.720787,16.197650,16.010270,16.367901,15.837011,16.351889,15.752036,16.163706,16.024721,16.414355,15.726812,20.413940,16.766970,16.519166,15.916114,15.370806,15.774706,16.956836,16.029521,16.534369,15.930838,15.885895,15.749020,15.589829,15.496842,15.751490,15.600833,16.085686,15.260866};



float voltage_see[XDL_HW][XBL_HW][WBL_HW][WDL_HW];
curr_obj goldenGate(float**** IMC_Out, mode_type mode, u8 slopeSetting, float* RefMax, int c, curr_obj currObj)
{
	scan_out_internal scanoutInternal[2];

	u8 ImgBank;
	ImgBank=0;

//	int numDLs=97;
//	int numWWLs=1092;
//	int numBLs=4;
//	u8 ***readSRAMTotal = (u8***)calloc(numDLs,sizeof(u8**));
//	for(uint16_t wd = 0; wd < numDLs; wd++){
//		readSRAMTotal[wd] = (u8**)calloc(numWWLs,sizeof(u8*));
//		for(uint16_t wl = 0; wl < numWWLs; wl++){
//			readSRAMTotal[wd][wl] = (u8*)calloc(numBLs,sizeof(u8));
//		}
//	}
//	currObj=readSRAMAll(currObj, readSRAMTotal);
//
//	u8* readSRAMTotal_FLAT = (u8*)calloc(numDLs*numWWLs*numBLs,sizeof(u8));
//
//	for(uint16_t wb = 0; wb < numBLs; wb++){
//		for(uint16_t wl = 0; wl < numWWLs; wl++){
//			for(uint16_t wd = 0; wd < numDLs; wd++){
//				readSRAMTotal_FLAT[wd + wl*( numDLs ) + wb*( numDLs*numWWLs ) ] = readSRAMTotal[wd][wl][wb] ;
//			}
//		}
//	}
//
//	char FileName_outI[]="RDSR.bin";
//	u32 size=numDLs*numWWLs*numBLs*sizeof(u8);
//	int rc=WriteFile_custom(FileName_outI, size, readSRAMTotal_FLAT);

//	char* readSRAMTotal_FLAT_RB = (char*)calloc(size,sizeof(char));
//	int Status = ReadFile( FileName_outI,(u32)readSRAMTotal_FLAT_RB);

//	Status=SD_Eject(&fatfs);
//	int pauseval=1;
// TIME MEASURING CODE STARTS
	XTime tStart_innerLoop;
	XTime tStop_innerLoop;
	XTime tElapsed_innerLoop;
	double tElapsed_innerLoop_msec;
// TIME MEASURING CODE ENDS


	scanoutInternal[ImgBank]=perform_IMC_Internal_pyTorch(currObj, ImgBank);
	//scanoutInternal[ImgBank]=perform_internalIMC_ImgBankFPGA(currObj, ImgBank);
	ImgBank=1;

	// TIME MEASURING CODE STARTS
		XTime_GetTime(&tStart_innerLoop);
	// TIME MEASURING CODE ENDS

	//currObj=init_internal(currObj);//293ms

	// TIME MEASURING CODE STARTS
		XTime_GetTime(&tStop_innerLoop);
		tElapsed_innerLoop=tStop_innerLoop-tStart_innerLoop;
		tElapsed_innerLoop_msec=1000*((double)tElapsed_innerLoop)/((double)COUNTS_PER_SECOND);
	// TIME MEASURING CODE ENDS

	scanoutInternal[ImgBank]=perform_IMC_Internal_pyTorch(currObj, ImgBank);
	//scanoutInternal[ImgBank]=perform_internalIMC_ImgBankFPGA(currObj, ImgBank);

	// ADC binary to ADC bin conversion
	for(int xdl=0;xdl<XDL_HW;xdl++)//IMAGE
	{
		ImgBank=(xdl>=4);
		for(int xbl=0;xbl<XBL_HW;xbl++)
		{
			for( int wbl=0 ; wbl<WBL_HW ; wbl++ )//Bitline
			{
				for( int wdl=0;wdl<WDL_HW;wdl++ )
					{

							float ADC_bin_see;
							u8 *ADCbits;
							ADCbits       = &scanoutInternal[ImgBank].scanOutBits_ADC_SC_IMGBL[ 0 + ADC_outs_total[map_to_72[wdl]] * ADC_nbits + scan_len_ADC_SC * (wbl + (xdl%4) * 4)];
							ADC_bin_see  =   scanoutInternal[ImgBank].scanOutBits_ADC_SC_IMGBL[ 0 + ADC_outs_total[map_to_72[wdl]] * ADC_nbits + scan_len_ADC_SC * (wbl + (xdl%4) * 4)] ;//
						for( int b=1;b<ADC_nbits;b++)
						{
							ADC_bin_see += ((scanoutInternal[ImgBank].scanOutBits_ADC_SC_IMGBL[ b + ADC_outs_total[map_to_72[wdl]] * ADC_nbits + scan_len_ADC_SC * (wbl + (xdl%4) * 4)])<<b);
						}
						ADC_bins[xdl][xbl][wbl][wdl]=ADC_bin_see;


					}
				if(mode.mode==mode_ICQ){
					Addertree_dec_ADC[xdl][wbl]=0;
					for( int b=0;b<Addertree_output_nbits;b++)
						Addertree_dec_ADC[xdl][wbl]+=(scanoutInternal[ImgBank].scanOutBits_AdderTree_SC[ b + wbl * ( Addertree_output_nbits ) + (xdl%4) * ( WBL_HW * Addertree_output_nbits ) ]<<b);
				}
			}
		}
	}

	float *ADC_bins_see;
	ADC_bins_see=ADC_bins;

	float ADC_mapped_bin;
	float voltage;
	float MAC;
	float ADC_bin;
//	u8 mode=2;
	// ADC bin->mapped bin->voltage->MAC

	if(mode.mode==mode_ICQ){
		// getting the central reference for a chunk
		for(int xdl=0;xdl<XDL_HW;xdl++)//XDL - number of images stored in SRAM
			{
			central_ref[xdl]=0;
			for( int wbl=0 ; wbl<WBL_HW ; wbl++ )//Bitline
				{
					central_ref[xdl]+=(Addertree_dec_ADC[xdl][wbl]<<wbl);
				}
			central_ref[xdl]=RefMax[c]*central_ref[xdl]/16;//corresponding to an LSB of xm*2^(-FL)
			}
	}

	float *m1R ;
	if(mode.mode==mode_ICQ){
		switch( slopeSetting ){
		case 1:{
					m1R = m1R_1X ;
					break;
				}
		case 2:{
					m1R = m1R_2X ;
					break;
				}
		case 4: {
					m1R = m1R_4X ;
					break;
				}
		}
	}


	for(int xdl=0;xdl<XDL_HW;xdl++)//XDL - number of images stored in SRAM
	{
		for(int xbl=0;xbl<XBL_HW;xbl++)//XBL - #bits to DAC
		{
			for(int wbl=0;wbl<WBL_HW;wbl++)//WBL - #BLs in chip
			{
				for(int wdl=0;wdl<WDL_HW;wdl++)//WDL - #usable DLs in chip
				{
					ADC_bin=ADC_bins[xdl][xbl][wbl][wdl];

					// SLOPE AND OFFSET SETTINGS #1
//					ADC_mapped_bin=ADC_bin * m_ADC[ADC_outs_total[wdl]] + c_ADC[ADC_outs_total[wdl]];
//					voltage = ( ADC_mapped_bin / 127 ) * 0.9 ;

//					// SLOPE AND OFFSET SETTINGS #2
//					ADC_mapped_bin = (ADC_bin-1)/2 * m_ADC[ADC_outs_total[wdl]] + c_ADC[ADC_outs_total[wdl]];
//					voltage = ( ADC_mapped_bin / 63 ) * 0.9 ; // *** MUST BE MADE / 64

					// SLOPE AND OFFSET SETTINGS #3
					ADC_mapped_bin=ADC_bin * m_ADC[ADC_outs_total[map_to_72[wdl]]] + c_ADC[ADC_outs_total[map_to_72[wdl]]];
					voltage = ( ADC_mapped_bin / 128 ) * 0.9 ; // for bin = k, voltage = 0 + k * V_LSB, where V_LSB = 0.9/128, thus divide by 128

					// extra added by ashwin on 20-Sep-2023 starts
					voltage_see[xdl][xbl][wbl][wdl]  =  voltage;
					// extra added by ashwin on 20-Sep-2023 ends
//					OLD mode_OCC code starts here
//					if( mode.mode == mode_OCC ){
//						//MAC = 0.86*( voltage / 0.9 ) * 160 ;
//						MAC = 0.97 * ( voltage / 0.9 ) * 166.5 ;
//					}
//					OLD mode_OCC code ends here
					if( mode.mode == mode_OCC ){
						float LSB=mode.LSB;
						float nbit_ADC=mode.nbit_ADC;
						float slopecorrect=mode.slopecorrect_chip;
						float vdd=mode.vdd;

						float OCC=mode.LSB*(pow(2,nbit_ADC)-1);
						MAC = slopecorrect * ( voltage / vdd ) * OCC ;
					}
					if( mode.mode==mode_ICQ ){
//						float deltaMAC;
//						float LSB=1.59;
//						float LC=-26;
//						float k=0.5222E-6;
//						float slopecorrect=0.7945;

						float LSB=mode.LSB;
//						float LC=mode.LC;
						float LC=-(LT[map_to_72[wdl]]+2.1);// old value is 16.9 -> new value is 19 -> change is +2.1
						float I_mean=mode.I_mean;
						// float slopecorrect=mode.slopecorrect;
						float slopecorrect=mode.slopecorrect_chip;

						LC = ( ( central_ref[xdl] + LC ) > 0 ) ? LC : ( -central_ref[xdl] ) ;
						// one gets 0 V to 0.9 V for deltaMAC of LC to LC+63*LSB
						// y = y1 + (x-x1)*( y2 - y1 )/( x2 - x1 )
						// deltaMAC =  LC + ( ( voltage - 0 ) * ( ( ( 63*LSB + LC ) - ( LC ) ) / ( ( 0.9 ) - ( 0 ) ) ) ) ;
//						deltaMAC =  LC + ( voltage * 63*LSB / 0.9 ) ;
//						MAC = deltaMAC + central_ref[xdl] ;



						MAC =  ( m2[map_to_72[wdl]]*( central_ref[xdl] + LC ) ) + ( voltage  / ( m1R[map_to_72[wdl]] * I_mean ) ) ;
						MAC = ( MAC >= 0 ) ? MAC : 0 ; // MAC = min( MAC, 0 )
						MAC = slopecorrect * MAC ;
					}
					IMC_Out[xdl][wdl][xbl][wbl]      =  (float) MAC;
//					IMC_Out_see[xdl][xbl][wbl][wdl]  =  (uint16_t) MAC;
				}
			}
		}
	}
	return currObj;
}

float ADCbin_to_mapped_bin( float ADC_bin, int wdl )
{
//	float ADC_mapped_bin = ADC_bin * m[ADC_outs_total[wdl]] + c_ADC[ADC_outs_total[wdl]];
	float ADC_mapped_bin = ADC_bin ;
	return ADC_mapped_bin;
}
float mapped_bin_to_voltage( float ADC_mapped_bin )
{
	float voltage = ( ADC_mapped_bin / 127 ) * 0.9 ;
	return voltage ;
}
float voltage_to_MAC( float voltage, u8 mode )
{
	float MAC;
	if( mode == 2 ){
		MAC = ( voltage / 0.9 ) * 160 ;
	}
	return MAC ;
}
