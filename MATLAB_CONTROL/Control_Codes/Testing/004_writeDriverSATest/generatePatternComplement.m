function pattern = generatePatternComplement(length)
    pattern = repmat('01',1,ceil(length/2));
    pattern = pattern(1:length);
    pattern = pattern-'0';
end