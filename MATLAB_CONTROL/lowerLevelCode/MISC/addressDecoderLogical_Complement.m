function vecScanIn = addressDecoderLogical_Complement(number)
    vecScanIn=ones(1,1092);
    vecScanIn(number+1)=0;
end