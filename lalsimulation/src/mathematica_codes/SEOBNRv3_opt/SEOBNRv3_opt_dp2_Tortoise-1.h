REAL8 tmp1=sigmaKerr->data[0]*sigmaKerr->data[0];
REAL8 tmp2=sigmaKerr->data[1]*sigmaKerr->data[1];
REAL8 tmp3=sigmaKerr->data[2]*sigmaKerr->data[2];
REAL8 tmp6=x->data[0]*x->data[0];
REAL8 tmp7=x->data[1]*x->data[1];
REAL8 tmp8=x->data[2]*x->data[2];
REAL8 tmp4=tmp1+tmp2+tmp3;
REAL8 tmp9=tmp6+tmp7+tmp8;
REAL8 tmp12=coeffs->KK*eta;
REAL8 tmp13=-1.+tmp12;
REAL8 tmp19=1./sqrt(tmp9);
REAL8 tmp16=1/tmp9;
REAL8 tmp30=pow(tmp9,-2.5);
REAL8 tmp10=tmp1+tmp2+tmp3+tmp6+tmp7+tmp8;
REAL8 tmp14=(1.0/(tmp13*tmp13));
REAL8 tmp15=1.*tmp14;
REAL8 tmp17=1.*tmp16*tmp4;
REAL8 tmp18=1/tmp13;
REAL8 tmp20=2.*tmp18*tmp19;
REAL8 tmp21=tmp15+tmp17+tmp20;
REAL8 tmp34=(1.0/sqrt(tmp9*tmp9*tmp9));
REAL8 tmp29=coeffs->k0*eta;
REAL8 tmp31=1.*coeffs->k5*tmp30;
REAL8 tmp32=(1.0/(tmp9*tmp9));
REAL8 tmp33=1.*coeffs->k4*tmp32;
REAL8 tmp35=1.*coeffs->k3*tmp34;
REAL8 tmp36=1.*coeffs->k2*tmp16;
REAL8 tmp37=1.*coeffs->k1*tmp19;
REAL8 tmp38=1.*tmp19;
REAL8 tmp39=log(tmp38);
REAL8 tmp40=1.*coeffs->k5l*tmp30*tmp39;
REAL8 tmp41=1.+tmp31+tmp33+tmp35+tmp36+tmp37+tmp40;
REAL8 tmp42=log(tmp41);
REAL8 tmp43=eta*tmp42;
REAL8 tmp44=1.+tmp29+tmp43;
REAL8 tmp51=tmp9*tmp9;
REAL8 tmp52=tmp21*tmp21;
REAL8 tmp53=-3.*eta;
REAL8 tmp54=26.+tmp53;
REAL8 tmp55=2.*eta*tmp34*tmp54;
REAL8 tmp56=6.*eta*tmp16;
REAL8 tmp57=1.+tmp55+tmp56;
REAL8 tmp58=log(tmp57);
REAL8 tmp59=1.+tmp58;
REAL8 tmp60=tmp44*tmp44;
REAL8 tmp61=tmp51*tmp52*tmp59*tmp60;
REAL8 tmp62=1./sqrt(tmp61);
REAL8 tmp63=1/tmp10;
REAL8 tmp64=sqrt(tmp61);
REAL8 tmp65=tmp63*tmp64;
REAL8 tmp66=-1.+tmp65;
REAL8 tmp22=e3_x*tmp19*x->data[0];
REAL8 tmp23=e3_y*tmp19*x->data[1];
REAL8 tmp24=e3_z*tmp19*x->data[2];
REAL8 tmp25=tmp22+tmp23+tmp24;
REAL8 tmp26=tmp25*tmp25;
REAL8 tmp11=tmp10*tmp10;
REAL8 tmp27=-tmp26;
REAL8 tmp28=1.+tmp27;
REAL8 tmp45=-(tmp21*tmp28*tmp4*tmp44*tmp9);
REAL8 tmp46=tmp11+tmp45;
REAL8 tmp81=tmp26*tmp4;
REAL8 tmp82=tmp6+tmp7+tmp8+tmp81;
REAL8 tmp83=1/tmp82;
REAL8 tmp75=-(tmp10*tmp16*tmp62*tmp66*tmp8);
REAL8 tmp76=1.+tmp75;
REAL8 tmp88=p->data[0]*tmp19*x->data[0];
REAL8 tmp89=p->data[1]*tmp19*x->data[1];
REAL8 tmp90=p->data[2]*tmp19*x->data[2];
REAL8 tmp91=tmp88+tmp89+tmp90;
REAL8 tmp47=1/tmp46;
REAL8 tmp48=e3_z*tmp19*x->data[0];
REAL8 tmp49=-(e3_x*tmp19*x->data[2]);
REAL8 tmp50=tmp48+tmp49;
REAL8 tmp67=-(tmp10*tmp16*tmp50*tmp62*tmp66*x->data[1]*x->data[2]);
REAL8 tmp68=-(e3_z*tmp19*x->data[1]);
REAL8 tmp69=e3_y*tmp19*x->data[2];
REAL8 tmp70=tmp68+tmp69;
REAL8 tmp71=-(tmp10*tmp16*tmp62*tmp66*tmp70*x->data[0]*x->data[2]);
REAL8 tmp72=-(e3_y*tmp19*x->data[0]);
REAL8 tmp73=e3_x*tmp19*x->data[1];
REAL8 tmp74=tmp72+tmp73;
REAL8 tmp77=tmp74*tmp76;
REAL8 tmp78=tmp67+tmp71+tmp77;
REAL8 tmp98=-(tmp10*tmp19*tmp62*tmp66*tmp91*x->data[0]);
REAL8 tmp99=p->data[0]+tmp98;
REAL8 tmp101=-(tmp10*tmp19*tmp62*tmp66*tmp91*x->data[1]);
REAL8 tmp102=p->data[1]+tmp101;
REAL8 tmp104=-(tmp10*tmp19*tmp62*tmp66*tmp91*x->data[2]);
REAL8 tmp105=p->data[2]+tmp104;
REAL8 tmp109=1/tmp28;
REAL8 tmp115=tmp19*tmp74*x->data[1];
REAL8 tmp116=-(tmp19*tmp50*x->data[2]);
REAL8 tmp117=tmp115+tmp116;
REAL8 tmp119=-(tmp19*tmp74*x->data[0]);
REAL8 tmp120=tmp19*tmp70*x->data[2];
REAL8 tmp121=tmp119+tmp120;
REAL8 tmp123=tmp19*tmp50*x->data[0];
REAL8 tmp124=-(tmp19*tmp70*x->data[1]);
REAL8 tmp125=tmp123+tmp124;
REAL8 tmp87=4.+tmp53;
REAL8 tmp100=tmp19*tmp99*x->data[0];
REAL8 tmp103=tmp102*tmp19*x->data[1];
REAL8 tmp106=tmp105*tmp19*x->data[2];
REAL8 tmp107=tmp100+tmp103+tmp106;
REAL8 tmp110=tmp70*tmp99;
REAL8 tmp111=tmp102*tmp50;
REAL8 tmp112=tmp105*tmp74;
REAL8 tmp113=tmp110+tmp111+tmp112;
REAL8 tmp128=tmp117*tmp99;
REAL8 tmp129=tmp102*tmp121;
REAL8 tmp130=tmp105*tmp125;
REAL8 tmp131=tmp128+tmp129+tmp130;
REAL8 tmp5=sqrt(tmp4);
REAL8 tmp94=-(tmp10*tmp34*tmp6*tmp62*tmp66*x->data[2]);
REAL8 tmp95=-(tmp10*tmp34*tmp62*tmp66*tmp7*x->data[2]);
REAL8 tmp96=tmp19*tmp76*x->data[2];
REAL8 tmp97=tmp94+tmp95+tmp96;
REAL8 tmp146=sqrt(tmp9*tmp9*tmp9);
REAL8 tmp145=sqrt(tmp9);
REAL8 tmp108=2.*tmp107*tmp21*tmp44*tmp59*tmp83*tmp9*tmp97;
REAL8 tmp114=2.*tmp109*tmp113*tmp47*tmp78*tmp82*tmp9;
REAL8 tmp118=-(tmp10*tmp117*tmp16*tmp62*tmp66*x->data[0]*x->data[2]);
REAL8 tmp122=-(tmp10*tmp121*tmp16*tmp62*tmp66*x->data[1]*x->data[2]);
REAL8 tmp126=tmp125*tmp76;
REAL8 tmp127=tmp118+tmp122+tmp126;
REAL8 tmp132=2.*tmp109*tmp127*tmp131*tmp83*tmp9;
REAL8 tmp149=tmp108+tmp114+tmp132;
REAL8 tmp154=eta*eta;
REAL8 tmp136=tmp107*tmp107;
REAL8 tmp137=tmp136*tmp21*tmp44*tmp59*tmp83*tmp9;
REAL8 tmp138=tmp113*tmp113;
REAL8 tmp139=tmp109*tmp138*tmp47*tmp82*tmp9;
REAL8 tmp140=tmp131*tmp131;
REAL8 tmp141=tmp109*tmp140*tmp83*tmp9;
REAL8 tmp164=-16.*eta;
REAL8 tmp165=21.*tmp154;
REAL8 tmp166=tmp164+tmp165;
REAL8 tmp170=0.+tmp137+tmp139+tmp141;
REAL8 tmp155=(tmp9*tmp9*tmp9);
REAL8 tmp156=(1.0/(tmp82*tmp82));
REAL8 tmp157=tmp59*tmp59;
REAL8 tmp158=(tmp107*tmp107*tmp107);
REAL8 tmp184=-6.*eta;
REAL8 tmp185=39.*tmp154;
REAL8 tmp186=tmp184+tmp185;
REAL8 tmp159=-1440.*tmp154*tmp155*tmp156*tmp157*tmp158*tmp52*tmp60*tmp97;
REAL8 tmp160=103.*eta;
REAL8 tmp161=-60.*tmp154;
REAL8 tmp162=tmp160+tmp161;
REAL8 tmp163=2.*tmp145*tmp149*tmp162;
REAL8 tmp167=6.*tmp136*tmp149*tmp166*tmp21*tmp44*tmp51*tmp59*tmp83;
REAL8 tmp168=3.*eta;
REAL8 tmp169=23.+tmp168;
REAL8 tmp171=2.*eta*tmp149*tmp169*tmp170*tmp9;
REAL8 tmp172=-47.*eta;
REAL8 tmp173=54.*tmp154;
REAL8 tmp174=tmp145*tmp166*tmp170;
REAL8 tmp175=tmp172+tmp173+tmp174;
REAL8 tmp176=12.*tmp107*tmp146*tmp175*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp177=tmp159+tmp163+tmp167+tmp171+tmp176;
REAL8 tmp179=3240.*tmp154*tmp155*tmp156*tmp157*tmp158*tmp52*tmp60*tmp97;
REAL8 tmp180=-109.*eta;
REAL8 tmp181=51.*tmp154;
REAL8 tmp182=tmp180+tmp181;
REAL8 tmp183=4.*tmp145*tmp149*tmp182;
REAL8 tmp187=-6.*tmp136*tmp149*tmp186*tmp21*tmp44*tmp51*tmp59*tmp83;
REAL8 tmp188=-90.*eta*tmp149*tmp170*tmp9;
REAL8 tmp189=16.*eta;
REAL8 tmp190=147.*tmp154;
REAL8 tmp191=tmp145*tmp170*tmp186;
REAL8 tmp192=tmp189+tmp190+tmp191;
REAL8 tmp193=-12.*tmp107*tmp146*tmp192*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp194=tmp179+tmp183+tmp187+tmp188+tmp193;
REAL8 tmp220=sqrt(tmp82);
REAL8 tmp147=-72.*sigmaKerr->data[0]*tmp107*tmp146*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp148=-60.*sigmaStar->data[0]*tmp107*tmp146*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp150=3.*sigmaKerr->data[0]*tmp145*tmp149;
REAL8 tmp151=4.*sigmaStar->data[0]*tmp145*tmp149;
REAL8 tmp152=tmp147+tmp148+tmp150+tmp151;
REAL8 tmp153=0.08333333333333333*eta*tmp152*tmp19;
REAL8 tmp178=-0.013888888888888888*sigmaStar->data[0]*tmp16*tmp177;
REAL8 tmp195=0.006944444444444444*sigmaKerr->data[0]*tmp16*tmp194;
REAL8 tmp196=tmp153+tmp178+tmp195;
REAL8 tmp197=e3_x*tmp196;
REAL8 tmp198=-72.*sigmaKerr->data[1]*tmp107*tmp146*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp199=-60.*sigmaStar->data[1]*tmp107*tmp146*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp200=3.*sigmaKerr->data[1]*tmp145*tmp149;
REAL8 tmp201=4.*sigmaStar->data[1]*tmp145*tmp149;
REAL8 tmp202=tmp198+tmp199+tmp200+tmp201;
REAL8 tmp203=0.08333333333333333*eta*tmp19*tmp202;
REAL8 tmp204=-0.013888888888888888*sigmaStar->data[1]*tmp16*tmp177;
REAL8 tmp205=0.006944444444444444*sigmaKerr->data[1]*tmp16*tmp194;
REAL8 tmp206=tmp203+tmp204+tmp205;
REAL8 tmp207=e3_y*tmp206;
REAL8 tmp208=-72.*sigmaKerr->data[2]*tmp107*tmp146*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp209=-60.*sigmaStar->data[2]*tmp107*tmp146*tmp21*tmp44*tmp59*tmp83*tmp97;
REAL8 tmp210=3.*sigmaKerr->data[2]*tmp145*tmp149;
REAL8 tmp211=4.*sigmaStar->data[2]*tmp145*tmp149;
REAL8 tmp212=tmp208+tmp209+tmp210+tmp211;
REAL8 tmp213=0.08333333333333333*eta*tmp19*tmp212;
REAL8 tmp214=-0.013888888888888888*sigmaStar->data[2]*tmp16*tmp177;
REAL8 tmp215=0.006944444444444444*sigmaKerr->data[2]*tmp16*tmp194;
REAL8 tmp216=tmp213+tmp214+tmp215;
REAL8 tmp217=e3_z*tmp216;
REAL8 tmp218=tmp197+tmp207+tmp217;
REAL8 tmp221=tmp21*tmp44*tmp9;
REAL8 tmp222=sqrt(tmp221);
REAL8 tmp223=-tmp222;
REAL8 tmp224=tmp21*tmp44*tmp47*tmp82*tmp9;
REAL8 tmp225=sqrt(tmp224);
REAL8 tmp226=tmp220*tmp225;
REAL8 tmp227=tmp223+tmp226;
REAL8 tmp228=1.+tmp137+tmp139+tmp141;
REAL8 tmp244=(tmp107*tmp107*tmp107*tmp107);
REAL8 tmp247=tmp170*tmp170;
REAL8 tmp241=27.*eta;
REAL8 tmp242=-353.+tmp241;
REAL8 tmp243=2.*eta*tmp242;
REAL8 tmp245=-360.*tmp154*tmp155*tmp156*tmp157*tmp244*tmp52*tmp60;
REAL8 tmp246=2.*tmp145*tmp162*tmp170;
REAL8 tmp248=eta*tmp169*tmp247*tmp9;
REAL8 tmp249=6.*tmp136*tmp146*tmp175*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp250=tmp243+tmp245+tmp246+tmp248+tmp249;
REAL8 tmp252=8.+tmp168;
REAL8 tmp253=-112.*eta*tmp252;
REAL8 tmp254=810.*tmp154*tmp155*tmp156*tmp157*tmp244*tmp52*tmp60;
REAL8 tmp255=4.*tmp145*tmp170*tmp182;
REAL8 tmp256=-45.*eta*tmp247*tmp9;
REAL8 tmp257=-6.*tmp136*tmp146*tmp192*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp258=tmp253+tmp254+tmp255+tmp256+tmp257;
REAL8 tmp229=1./sqrt(tmp228);
REAL8 tmp232=coeffs->d1v2*eta*sigmaKerr->data[0]*tmp34;
REAL8 tmp233=-8.*sigmaKerr->data[0];
REAL8 tmp234=14.*sigmaStar->data[0];
REAL8 tmp235=-36.*sigmaKerr->data[0]*tmp136*tmp146*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp236=-30.*sigmaStar->data[0]*tmp136*tmp146*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp237=3.*sigmaKerr->data[0]*tmp145*tmp170;
REAL8 tmp238=4.*sigmaStar->data[0]*tmp145*tmp170;
REAL8 tmp239=tmp233+tmp234+tmp235+tmp236+tmp237+tmp238;
REAL8 tmp240=0.08333333333333333*eta*tmp19*tmp239;
REAL8 tmp251=-0.013888888888888888*sigmaStar->data[0]*tmp16*tmp250;
REAL8 tmp259=0.006944444444444444*sigmaKerr->data[0]*tmp16*tmp258;
REAL8 tmp260=sigmaStar->data[0]+tmp232+tmp240+tmp251+tmp259;
REAL8 tmp261=e3_x*tmp260;
REAL8 tmp262=coeffs->d1v2*eta*sigmaKerr->data[1]*tmp34;
REAL8 tmp263=-8.*sigmaKerr->data[1];
REAL8 tmp264=14.*sigmaStar->data[1];
REAL8 tmp265=-36.*sigmaKerr->data[1]*tmp136*tmp146*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp266=-30.*sigmaStar->data[1]*tmp136*tmp146*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp267=3.*sigmaKerr->data[1]*tmp145*tmp170;
REAL8 tmp268=4.*sigmaStar->data[1]*tmp145*tmp170;
REAL8 tmp269=tmp263+tmp264+tmp265+tmp266+tmp267+tmp268;
REAL8 tmp270=0.08333333333333333*eta*tmp19*tmp269;
REAL8 tmp271=-0.013888888888888888*sigmaStar->data[1]*tmp16*tmp250;
REAL8 tmp272=0.006944444444444444*sigmaKerr->data[1]*tmp16*tmp258;
REAL8 tmp273=sigmaStar->data[1]+tmp262+tmp270+tmp271+tmp272;
REAL8 tmp274=e3_y*tmp273;
REAL8 tmp275=coeffs->d1v2*eta*sigmaKerr->data[2]*tmp34;
REAL8 tmp276=-8.*sigmaKerr->data[2];
REAL8 tmp277=14.*sigmaStar->data[2];
REAL8 tmp278=-36.*sigmaKerr->data[2]*tmp136*tmp146*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp279=-30.*sigmaStar->data[2]*tmp136*tmp146*tmp21*tmp44*tmp59*tmp83;
REAL8 tmp280=3.*sigmaKerr->data[2]*tmp145*tmp170;
REAL8 tmp281=4.*sigmaStar->data[2]*tmp145*tmp170;
REAL8 tmp282=tmp276+tmp277+tmp278+tmp279+tmp280+tmp281;
REAL8 tmp283=0.08333333333333333*eta*tmp19*tmp282;
REAL8 tmp284=-0.013888888888888888*sigmaStar->data[2]*tmp16*tmp250;
REAL8 tmp285=0.006944444444444444*sigmaKerr->data[2]*tmp16*tmp258;
REAL8 tmp286=sigmaStar->data[2]+tmp275+tmp283+tmp284+tmp285;
REAL8 tmp287=e3_z*tmp286;
REAL8 tmp288=tmp261+tmp274+tmp287;
REAL8 tmp310=sqrt(tmp228);
REAL8 tmp294=tmp19*tmp196*x->data[0];
REAL8 tmp295=tmp19*tmp206*x->data[1];
REAL8 tmp296=tmp19*tmp216*x->data[2];
REAL8 tmp297=tmp294+tmp295+tmp296;
REAL8 tmp313=tmp82*tmp82;
REAL8 tmp298=tmp19*tmp260*x->data[0];
REAL8 tmp299=tmp19*tmp273*x->data[1];
REAL8 tmp300=tmp19*tmp286*x->data[2];
REAL8 tmp301=tmp298+tmp299+tmp300;
REAL8 tmp331=tmp21*tmp44*tmp59*tmp9;
REAL8 tmp332=sqrt(tmp331);
REAL8 tmp343=tmp260*tmp70;
REAL8 tmp344=tmp273*tmp50;
REAL8 tmp345=tmp286*tmp74;
REAL8 tmp346=tmp343+tmp344+tmp345;
REAL8 tmp348=tmp117*tmp260;
REAL8 tmp349=tmp121*tmp273;
REAL8 tmp350=tmp125*tmp286;
REAL8 tmp351=tmp348+tmp349+tmp350;
REAL8 tmp305=sqrt(tmp4*tmp4*tmp4);
REAL8 tmp306=(1.0/sqrt(tmp82*tmp82*tmp82));
REAL8 tmp307=1./sqrt(tmp221);
REAL8 tmp308=1./sqrt(tmp224);
REAL8 tmp309=(1.0/(tmp46*tmp46));
REAL8 tmp311=1.+tmp310;
REAL8 tmp314=-(tmp138*tmp21*tmp313*tmp44*tmp47*tmp51);
REAL8 tmp315=tmp140*tmp9;
REAL8 tmp316=1.+tmp137+tmp139+tmp141+tmp310;
REAL8 tmp317=-(tmp28*tmp316*tmp82);
REAL8 tmp318=tmp315+tmp317;
REAL8 tmp319=tmp21*tmp318*tmp44*tmp9;
REAL8 tmp320=tmp314+tmp319;
REAL8 tmp355=-(tmp113*tmp145*tmp220*tmp225*tmp346);
REAL8 tmp356=tmp131*tmp145*tmp222*tmp351;
REAL8 tmp357=tmp355+tmp356;
REAL8 tmp231=(1.0/sqrt(tmp228*tmp228*tmp228));
REAL8 tmp312=1/tmp311;
REAL8 tmp363=tmp301*tmp320;
REAL8 tmp364=-(tmp107*tmp222*tmp332*tmp357);
REAL8 tmp365=tmp363+tmp364;
REAL8 tmp370=coeffs->k5l*tmp39;
REAL8 tmp371=coeffs->k5+tmp370;
REAL8 tmp333=tmp196*tmp70;
REAL8 tmp334=tmp206*tmp50;
REAL8 tmp335=tmp216*tmp74;
REAL8 tmp336=tmp333+tmp334+tmp335;
REAL8 tmp338=tmp117*tmp196;
REAL8 tmp339=tmp121*tmp206;
REAL8 tmp340=tmp125*tmp216;
REAL8 tmp341=tmp338+tmp339+tmp340;
REAL8 tmp324=0.5*tmp149*tmp229;
REAL8 tmp325=tmp108+tmp114+tmp132+tmp324;
REAL8 tmp368=2.*tmp46*tmp5;
REAL8 tmp369=4.*tmp10*tmp145;
REAL8 tmp372=1.*tmp30*tmp371;
REAL8 tmp373=1.+tmp33+tmp35+tmp36+tmp37+tmp372;
REAL8 tmp374=1/tmp373;
REAL8 tmp375=2.*coeffs->k2;
REAL8 tmp376=3.*coeffs->k3;
REAL8 tmp377=4.*coeffs->k4;
REAL8 tmp378=5.*tmp19*tmp371;
REAL8 tmp379=tmp377+tmp378;
REAL8 tmp380=1.*tmp19*tmp379;
REAL8 tmp381=tmp376+tmp380;
REAL8 tmp382=1.*tmp19*tmp381;
REAL8 tmp383=tmp375+tmp382;
REAL8 tmp384=1.*tmp19*tmp383;
REAL8 tmp385=coeffs->k1+tmp384;
REAL8 tmp386=-(eta*tmp21*tmp374*tmp385);
REAL8 tmp387=2.*tmp145*tmp21*tmp44;
REAL8 tmp388=1.*tmp18;
REAL8 tmp389=1.*tmp19*tmp4;
REAL8 tmp390=tmp388+tmp389;
REAL8 tmp391=-2.*tmp390*tmp44;
REAL8 tmp392=tmp386+tmp387+tmp391;
REAL8 tmp393=-(tmp28*tmp392*tmp4);
REAL8 tmp394=tmp369+tmp393;
REAL8 tmp395=-2.*tmp145*tmp394*tmp5;
REAL8 tmp396=tmp368+tmp395;
REAL8 tmp361=1/tmp228;
REAL8 tmp362=(1.0/(tmp311*tmp311));
REAL8 tmp413=-(tmp113*tmp131*tmp220*tmp222*tmp225*tmp346*tmp9);
REAL8 tmp414=tmp138*tmp21*tmp313*tmp351*tmp44*tmp47*tmp51;
REAL8 tmp415=tmp107*tmp131*tmp145*tmp301*tmp332;
REAL8 tmp416=-(tmp136*tmp21*tmp351*tmp44*tmp59*tmp9);
REAL8 tmp417=tmp316*tmp351*tmp82;
REAL8 tmp418=tmp415+tmp416+tmp417;
REAL8 tmp419=tmp21*tmp28*tmp418*tmp44*tmp9;
REAL8 tmp420=tmp413+tmp414+tmp419;
REAL8 tmp80=1/tmp21;
REAL8 tmp84=1/tmp44;
REAL8 tmp424=1./sqrt(tmp82);
REAL8 tmp425=-(tmp21*tmp44*tmp9);
REAL8 tmp426=tmp1+tmp2+tmp3+tmp425+tmp6+tmp7+tmp8;
REAL8 tmp427=2.*tmp310;
REAL8 tmp428=1.+tmp427;
REAL8 tmp432=-2.*tmp21*tmp44*tmp9;
REAL8 tmp433=tmp332*tmp392;
REAL8 tmp434=tmp432+tmp433;
REAL8 tmp438=tmp145*tmp83;
REAL8 tmp439=1./sqrt(tmp331);
REAL8 tmp440=-tmp439;
REAL8 tmp441=tmp438+tmp440;
REAL8 tmp449=-4.*tmp146*tmp21*tmp44;
REAL8 tmp450=tmp10*tmp392;
REAL8 tmp451=tmp449+tmp450;
REAL8 tmp452=0.5*tmp10*tmp16*tmp451*tmp47*tmp80*tmp84;
REAL8 tmp453=tmp438+tmp452;
REAL8 tmp444=-(tmp25*tmp4*tmp83);
REAL8 tmp445=tmp10*tmp25*tmp4*tmp426*tmp47*tmp83;
REAL8 tmp446=tmp444+tmp445;
REAL8 tmp447=tmp107*tmp28*tmp446;
REAL8 tmp448=tmp131*tmp145*tmp441;
REAL8 tmp454=-(tmp131*tmp145*tmp453);
REAL8 tmp455=tmp447+tmp448+tmp454;
REAL8 tmp443=-(tmp107*tmp25*tmp28*tmp4*tmp83);
REAL8 tmp456=tmp310*tmp455;
REAL8 tmp457=tmp443+tmp456;
REAL8 tmp85=tmp16*tmp46*tmp80*tmp83*tmp84;
REAL8 tmp86=1./sqrt(tmp85);
REAL8 tmp134=(tmp91*tmp91*tmp91*tmp91);
REAL8 tmp135=2.*eta*tmp134*tmp16*tmp87;
REAL8 tmp142=1.+tmp135+tmp137+tmp139+tmp141;
REAL8 tmp423=1/tmp316;
REAL8 tmp479=-(tmp10*tmp113*tmp145*tmp222*tmp225*tmp25*tmp28*tmp301*tmp4*tmp424*tmp426*tmp428*tmp47);
REAL8 tmp480=-0.5*tmp113*tmp145*tmp220*tmp225*tmp311*tmp351*tmp434*tmp62;
REAL8 tmp481=tmp131*tmp145*tmp222*tmp346*tmp441;
REAL8 tmp482=tmp222*tmp346*tmp457;
REAL8 tmp483=tmp113*tmp145*tmp220*tmp225*tmp351*tmp428*tmp453;
REAL8 tmp484=tmp481+tmp482+tmp483;
REAL8 tmp485=tmp222*tmp484;
REAL8 tmp486=tmp480+tmp485;
REAL8 tmp487=tmp332*tmp486;
REAL8 tmp488=tmp479+tmp487;
REAL8 d000001=(1.*eta*(-0.5*(2.*tmp196*tmp260+2.*tmp206*tmp273+2.*tmp216*tmp286-6.*tmp297*tmp301)*tmp34-0.25*tmp109*tmp149*tmp231*tmp306*tmp307*tmp308*tmp309*tmp312*tmp332*tmp396*tmp420-0.25*tmp109*tmp149*tmp306*tmp307*tmp308*tmp309*tmp332*tmp361*tmp362*tmp396*tmp420+1.*tmp146*tmp149*tmp21*tmp231*tmp25*tmp305*tmp306*tmp307*tmp308*tmp309*tmp312*tmp365*tmp44+1.*tmp146*tmp149*tmp21*tmp25*tmp305*tmp306*tmp307*tmp308*tmp309*tmp361*tmp362*tmp365*tmp44+tmp109*tmp113*tmp145*tmp218*tmp220*tmp227*tmp229*tmp47-0.5*tmp109*tmp113*tmp145*tmp149*tmp220*tmp227*tmp231*tmp288*tmp47+2.*tmp145*tmp218*tmp47*tmp5+tmp109*tmp145*tmp220*tmp227*tmp229*tmp288*tmp47*tmp78+2.*tmp47*tmp5*tmp78*tmp9-2.*tmp146*tmp21*tmp229*tmp25*tmp305*tmp306*tmp307*tmp308*tmp309*tmp312*tmp44*(tmp297*tmp320-tmp107*tmp222*tmp332*(-(tmp113*tmp145*tmp220*tmp225*tmp336)+tmp131*tmp145*tmp222*tmp341+tmp127*tmp145*tmp222*tmp351-tmp145*tmp220*tmp225*tmp346*tmp78)+tmp301*(-2.*tmp113*tmp21*tmp313*tmp44*tmp47*tmp51*tmp78+tmp21*tmp44*tmp9*(-(tmp28*tmp325*tmp82)+2.*tmp127*tmp131*tmp9))-tmp222*tmp332*tmp357*tmp97)+0.5*tmp109*tmp229*tmp306*tmp307*tmp308*tmp309*tmp312*tmp332*tmp396*(tmp138*tmp21*tmp313*tmp341*tmp44*tmp47*tmp51+2.*tmp113*tmp21*tmp313*tmp351*tmp44*tmp47*tmp51*tmp78-tmp113*tmp131*tmp220*tmp222*tmp225*tmp336*tmp9-tmp113*tmp127*tmp220*tmp222*tmp225*tmp346*tmp9-tmp131*tmp220*tmp222*tmp225*tmp346*tmp78*tmp9+tmp21*tmp28*tmp44*tmp9*(tmp107*tmp131*tmp145*tmp297*tmp332+tmp107*tmp127*tmp145*tmp301*tmp332+tmp316*tmp341*tmp82+tmp325*tmp351*tmp82-tmp136*tmp21*tmp341*tmp44*tmp59*tmp9+tmp131*tmp145*tmp301*tmp332*tmp97-2.*tmp107*tmp21*tmp351*tmp44*tmp59*tmp9*tmp97))+tmp109*tmp16*tmp225*tmp423*tmp80*tmp83*tmp84*(-(tmp10*tmp113*tmp145*tmp149*tmp222*tmp225*tmp229*tmp25*tmp28*tmp301*tmp4*tmp424*tmp426*tmp47)-tmp10*tmp113*tmp145*tmp222*tmp225*tmp25*tmp28*tmp297*tmp4*tmp424*tmp426*tmp428*tmp47-tmp10*tmp145*tmp222*tmp225*tmp25*tmp28*tmp301*tmp4*tmp424*tmp426*tmp428*tmp47*tmp78+tmp332*(-0.5*tmp113*tmp145*tmp220*tmp225*tmp311*tmp341*tmp434*tmp62-0.25*tmp113*tmp145*tmp149*tmp220*tmp225*tmp229*tmp351*tmp434*tmp62-0.5*tmp145*tmp220*tmp225*tmp311*tmp351*tmp434*tmp62*tmp78+tmp222*(tmp131*tmp145*tmp222*tmp336*tmp441+tmp127*tmp145*tmp222*tmp346*tmp441+1.*tmp113*tmp145*tmp149*tmp220*tmp225*tmp229*tmp351*tmp453+tmp113*tmp145*tmp220*tmp225*tmp341*tmp428*tmp453+tmp222*tmp336*tmp457+tmp145*tmp220*tmp225*tmp351*tmp428*tmp453*tmp78+tmp222*tmp346*(0.5*tmp149*tmp229*tmp455-tmp25*tmp28*tmp4*tmp83*tmp97+tmp310*(tmp127*tmp145*tmp441-tmp127*tmp145*tmp453+tmp28*tmp446*tmp97)))))-tmp109*tmp16*tmp225*tmp325*tmp488*tmp80*tmp83*tmp84*(1.0/(tmp316*tmp316))+(0.5*tmp86*(tmp108+tmp114+tmp132+8.*eta*tmp34*tmp87*x->data[2]*(tmp91*tmp91*tmp91)))/sqrt(tmp142)))/sqrt(1.+2.*eta*(-1.+0.5*tmp109*tmp229*tmp306*tmp307*tmp308*tmp309*tmp312*tmp332*tmp396*tmp420-2.*tmp146*tmp21*tmp229*tmp25*tmp305*tmp306*tmp307*tmp308*tmp309*tmp312*tmp365*tmp44+tmp109*tmp113*tmp145*tmp220*tmp227*tmp229*tmp288*tmp47+2.*tmp145*tmp288*tmp47*tmp5+tmp109*tmp16*tmp225*tmp423*tmp488*tmp80*tmp83*tmp84+2.*tmp113*tmp47*tmp5*tmp9+coeffs->dheffSSv2*eta*tmp32*(s1Vec->data[0]*s1Vec->data[0]+s1Vec->data[1]*s1Vec->data[1]+s1Vec->data[2]*s1Vec->data[2]+s2Vec->data[0]*s2Vec->data[0]+s2Vec->data[1]*s2Vec->data[1]+s2Vec->data[2]*s2Vec->data[2])-0.5*tmp34*(tmp260*tmp260+tmp273*tmp273+tmp286*tmp286-3.*(tmp301*tmp301))+tmp86*sqrt(tmp142)));
