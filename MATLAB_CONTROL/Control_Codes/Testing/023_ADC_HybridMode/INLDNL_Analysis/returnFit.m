function ADC_fit=returnFit(Nbits,fit_type,ADC_act, ADC_exp)
    eval( sprintf( '[ADC_fit, ~] = createFit_%s_%dbADC(ADC_act, ADC_exp);', fit_type, Nbits ) ) ;
end