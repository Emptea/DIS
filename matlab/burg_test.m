%%
clear
N = 32768 / 8;
n = (1:N)';
fs = 2e6;

fd = 181e3;
t = (0:N - 1)' / fs;
signal = 1 * cos(2 * pi * fd * t) .* (1 - 0.5 * cos(2 * pi * fd / 10 * t));
noise = 1 * randn(N, 1);
x = signal + noise;
[amp1, f1] = sp_fft(x, fs, 4);
[amp2, f2] = sp_class(x, fs, 4);
[amp3, f3] = sp_burg(x, 32, fs);
[amp4 , f4] = sp_burg_cur(x, 32, fs);
% [amp5, f5] = sp_burg_py(x, 2048, fs);
subplot(2, 1, 1)
plot(n, x)
title('Исходный ряд')
subplot(2, 1, 2)
hold on
plot(f1, amp1);
% plot(f2, amp2);
plot(f3, amp3);
plot(f4, amp4);
% plot(f5, amp5);
hold off
% plot(Amp(:, 1), Amp(:, 2), MMM(1:L, 1), MMM(1:L, 2), 'd', MarkerFaceColor = 'r', MarkerSize = 3)
grid on
title('Спектральный состав, БПФ')
%%
function [amp, f] = sp_fft(x, fs, mult)
    N = length(x);
    Nf = length(x) * mult;
    x_fft = fft(x, Nf);
    x_fft([1, end]) = 0;
    f = (0:Nf - 1) / Nf * fs;
    x_p = x_fft .* conj(x_fft);
    amp = 2 * x_p .^ 0.5 ./ N;
end

function [amp, f] = sp_class(x, fs, mult)
    N = length(x);
    Nf = mult * N;
    fnorm = (0:Nf - 1) / Nf;
    n = (1:N)';
    temp = zeros(Nf, 1);
    for i = 1:Nf
        temp(i) = sum(x .* exp(-1i * 2 * pi * fnorm(i) * n));
    end

    x_p = temp .* conj(temp);
    f = fnorm * fs;
    amp = 2 * x_p .^ 0.5 ./ N;
    % amp = power ;
end

function [amp, f] = sp_burg(x, pp, fs)
    p = round(pp);
    N = length(x);

    a = 1;
    ef = x;
    eb = x;
    E = zeros(p + 1, 1);
    E(1) = x' * x / N;
    K = zeros(p, 1);

    for i = 2:p + 1
        ep = ef(i:N);
        em = eb(i - 1:N - 1);
        K(i - 1) = ep' * em / (ep' * ep + em' * em);
        a = [a; 0] - K(i - 1) * [0; flipud(a)];

        for j = N:-1:i
            ef_old = ef(j);
            ef(j) = ef(j) - K(i - 1) * eb(j - 1);
            eb(j) = eb(j - 1) - K(i - 1) * ef_old;
        end

        E(i) = (1 - K(i - 1)' * K(i - 1)) * E(i - 1);
    end

    % Получили коэффициенты авторегрессии, теперь находим отклик фильтра
    % H(iw) = E(end) / fft(a, 256)
    % почему fft на 256 точек, это очень хороший вопрос для
    % самостоятельного изучения
    h = E(end) ./ (fft(a, 4*N));
    x_p = h .* conj(h);
    Nf = length(h);
    f = (0:Nf - 1) / Nf * fs;
    amp = 2 * x_p .^ 0.5;
    amp = amp / max(amp);
end

% Работает
function [amp , f] = sp_burg_cur(x, ORD, fs)
    len = length(x);
    ef = x;
    eb = x;

    Ak = [1; zeros(ORD, 1)];
    Dk = 0;

    for j = 1:len
        Dk = Dk + 2 * x(j) * x(j); % D0
    end

    Dk = Dk - (x(1) * x(1) + x(len) * x(len));
    for k = 1:ORD
        mu = 0;

        for i = 1:(len - ORD)
            mu = mu + ef(i + k) * eb(i);
        end

        mu = -2 * mu / Dk;
        
        % Ak(2:k+1) = Ak(2:k+1) + mu .* Ak(k:-1:1);
        for i = 2:k+1
            Ak(i) = Ak(i) + mu * Ak(k - (i - 2));
        end

        for i = 1:(len - k)
            eb_i = eb(i);
            eb(i) = eb_i + mu * ef(i + k);
            ef(i + k) = ef(i + k) + mu * eb_i;
        end

        f_sq = ef(k) * ef(k);
        b_sq = eb(len - k) * eb(len - k);
        Dk = (1 - mu * mu) * Dk - f_sq - b_sq;
    end
    y = filter(1,Ak,x);
    [amp , f] = sp_fft(y, fs, 1);
end

% это из библиотеки py вроде, совпадает c  sp_burg
function [amp, f] = sp_burg_py(x, ORDER, fs)
    N = length(x);
    rho = x' * conj(x) / N;

    ef = x;
    eb = x;

    a = 1;
    ref = zeros(ORDER, 1);
    E = zeros(ORDER + 1, 1);
    E(1) = rho;

    for m = 1:ORDER
        efp = ef(2:end);
        ebp = eb(1:end - 1);

        ref(m) = -2 * efp' * ebp / (efp' * efp + ebp' * ebp);
        ef = efp + ref(m) * ebp;
        eb = ebp + conj(ref(m))' * efp;

        a = [a; 0] + ref(m) * [0; flipud(a)];

        E(m + 1) = (1 - ref(m)' * ref(m)) * E(m);
    end

    [amp, f] = sp_fft(a, fs, 4);
end
