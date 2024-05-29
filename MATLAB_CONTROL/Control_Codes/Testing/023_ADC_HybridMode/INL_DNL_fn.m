function [INL,DNL]=INL_DNL_fn(n,curr)
    ydata=[0;curr];
    combs=2^n;
    delta=ydata(size(ydata,1))/((2^n)-1);
    DNL=zeros(size(ydata,1),1);
    INL=zeros(size(ydata,1),1);
    for i=1:size(ydata,1)-1
        DNL(i)=((ydata(i+1)-ydata(i))/delta)-1;
        INL(i)=(ydata(i)-((i-1)*delta))/delta;
    end
    INL(combs)=(ydata(combs)-((combs-1)*delta))/delta;
end