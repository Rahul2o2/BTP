% Execute device object function(s).
groupObj = get(instrumentObj, 'Instrumentspecifictransientarbitrarywaveformuserwaveformcurrent');
invoke(groupObj, 'userwaveformcurrentconfigurecurrentlist',1,2,[0,1E-6]);