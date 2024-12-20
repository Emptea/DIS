list = string(ls('test_*'));
fs = 2e6;
adc_resol = 16;
figure;
for i = 1:length(list)
    load(list(i))
    t = (0:length(res_fft)-1)/fs;
    f = (0:length(res_fft)-1)/length(res_fft)*fs;

    res_fft_db = mag2db(abs(res_fft./length(res_fft)/2^adc_resol));
    max_fft_db(:,i) = max(res_fft_db);
    hold on
    plot(f, res_fft_db)
    xlim([0 fs/2])
    title(list(i))
end