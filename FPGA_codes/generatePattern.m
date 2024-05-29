function pattern = generatePattern(length)
    pattern = repmat('10',1,ceil(length/2));
    pattern = pattern(1:length);
    pattern = pattern-'0';
end