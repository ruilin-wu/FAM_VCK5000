
# Running the Simulation

***Version: Matlab R2022a***

Use MATLAB software to open `test.mlx` and click the Run button.


## The principle and code implementation of FAM algorithm in Matlab

### Data Input
To match the input format in our AIE design, we store the input files in the [data_in](data_in) folder. We assume that the files stored in the [data_in](data_in) folder are files that have been processed by Input Channelization. At this time, the time domain sampling data is neatly arranged into a two-dimensional matrix with `P`(32) rows and `N'`(256) columns.

We convert the data from the input file into a 256*32 (transposed for ease of subsequent FFT calculations) 2D matrix in Matlab with the following code:
```
% Iterate through 8 input files
for ch = 0 : 7
    fname = fullfile('data_in', sprintf('FAMDataIn_%d.txt', ch));
    data  = dlmread(fname);  % Read file assuming format: real imag
    cdata = complex(data(:,1), data(:,2));  % Convert to complex vector of length 1024

    % Divide 1024 cfloat into 4 blocks (256 each), each block fills a column
    for blk = 0 : 3
        col_index = ch * 4 + blk + 1;  % Column index (1 to 32)
        start_idx = blk * 256 + 1;    % Start index
        end_idx   = (blk + 1) * 256;  % End index
        X(:, col_index) = cdata(start_idx : end_idx);  % Fill data into matrix
    end
end
```
Some critical aspects of the previous code are highlighted in the following:
* Each file stores 1024 single-precision complex numbers, which we divide into four 256-point chunks and compute to fill in the column indices of the matrix, resulting in a 256 × 32 matrix X with 256 complex numbers in each column.


### Windowing

A Hamming window is applied to each column of the array. The equation for the Hamming window is:

$$
w(r) = 0.54 - 0.46 \cos \left(\frac{2\pi r}{N' - 1}\right), \quad 0 \leq r \leq N' - 1 \quad (1)
$$

where \(N'\) represents the column length of the array.

We Windowing the data in the matrix with the following code:
```
a = [0.079999924,0.08013916,0.080558777,0.081256866,0.082231522,0.083486557,0.085018158,0.086826324,0.088907242,0.091264725,0.093893051,0.096792221,0.099962234,0.10339737,0.10709953,0.111063,0.11528778,0.11976814,0.124506,0.12949562,0.13473511,0.14021873,0.1459465,0.15191269,0.15811539,0.16454887,0.17121124,0.17809677,0.18520355,0.19252396,0.20005608,0.20779419,0.21573448,0.22387123,0.23220062,0.24071503,0.24941254,0.25828743,0.26733208,0.27654266,0.28591156,0.29543686,0.30510902,0.31492615,0.32487679,0.33496094,0.34516716,0.35549355,0.36593056,0.37647438,0.38711739,0.39785194,0.40867424,0.41957474,0.43054962,0.44159126,0.45269203,0.4638443,0.47504425,0.48628426,0.49755669,0.50885391,0.52017021,0.53149986,0.54283333,0.55416489,0.56548882,0.57679749,0.58808327,0.59934044,0.61055946,0.62173843,0.63286591,0.64393806,0.65494537,0.66588593,0.67674828,0.6875267,0.69821739,0.70881081,0.71930122,0.72968483,0.73995209,0.75009727,0.76011467,0.77000046,0.7797451,0.78934479,0.79879189,0.80808449,0.81721115,0.82617188,0.83495903,0.84356499,0.85198784,0.86022186,0.86826134,0.87610054,0.88373566,0.89116287,0.89837646,0.90537262,0.91214752,0.91869545,0.9250145,0.93109894,0.93694687,0.94255447,0.94791603,0.95302963,0.95789337,0.96250343,0.96685791,0.97095108,0.97478485,0.9783535,0.98165512,0.98468971,0.98745537,0.98994827,0.99216843,0.99411201,0.99578285,0.99717522,0.99829102,0.99912834,0.99968529,0.99996567,0.99996567,0.99968529,0.99912834,0.99829102,0.99717522,0.99578285,0.99411201,0.99216843,0.98994827,0.98745537,0.98468971,0.98165512,0.9783535,0.97478485,0.97095108,0.96685791,0.96250343,0.95789337,0.95302963,0.94791603,0.94255447,0.93694687,0.93109894,0.9250145,0.91869545,0.91214752,0.90537262,0.89837646,0.89116287,0.88373566,0.87610054,0.86826134,0.86022186,0.85198784,0.84356499,0.83495903,0.82617188,0.81721115,0.80808449,0.79879189,0.78934479,0.7797451,0.77000046,0.76011467,0.75009727,0.73995209,0.72968483,0.71930122,0.70881081,0.69821739,0.6875267,0.67674828,0.66588593,0.65494537,0.64393806,0.63286591,0.62173843,0.61055946,0.59934044,0.58808327,0.57679749,0.56548882,0.55416489,0.54283333,0.53149986,0.52017021,0.50885391,0.49755669,0.48628426,0.47504425,0.4638443,0.45269203,0.44159126,0.43054962,0.41957474,0.40867424,0.39785194,0.38711739,0.37647438,0.36593056,0.35549355,0.34516716,0.33496094,0.32487679,0.31492615,0.30510902,0.29543686,0.28591156,0.27654266,0.26733208,0.25828743,0.24941254,0.24071503,0.23220062,0.22387123,0.21573448,0.20779419,0.20005608,0.19252396,0.18520355,0.17809677,0.17121124,0.16454887,0.15811539,0.15191269,0.1459465,0.14021873,0.13473511,0.12949562,0.124506,0.11976814,0.11528778,0.111063,0.10709953,0.10339737,0.099962234,0.096792221,0.093893051,0.091264725,0.088907242,0.086826324,0.085018158,0.083486557,0.082231522,0.081256866,0.080558777,0.08013916,0.079999924];
a = fi(a,1,bit.windowing+2,bit.windowing+1);  % Convert window coefficients to fixed-point
XW=single(diag(a)*X);  % Apply window function to each column
result.Windowing = XW;
```
Some critical aspects of the previous code are highlighted in the following:

* Here `a` is a 256-dimensional array storing the coefficients of the Hamming window function. These coefficients are used in windowed FFT calculations to reduce spectral leakage.

* `diag(a)` creates a diagonal matrix with the Hamming window coefficients in `a` on the diagonal.

* `diag(a) * X` acts: `X` is a 256 × 32 complex matrix with each column representing the FFT input of a signal channel.
`diag(a) * X` multiplies the coefficients of `a` by each column of `X`, which is equivalent to weighting the window function for each channel.

### First FFT (256-pt)
Each column of the windowed data array is Fast Fourier transformed to reveal the first spectral components. The resultant array is still indexed P rows by N' columns but now the column index relates to a specific bin of spectral frequencies. 

The matlab code of the FFT function is shown below:
```
function OutputSignal = FFTFloatv3_M1(InputSignal)
% FFTFloatv3_M1 performs FFT computation on single-precision floating-point data
% Input:
%   InputSignal - input signal matrix (rows: signal length, columns: time samples)
% Output:
%   OutputSignal - FFT-transformed signal matrix
S = double(InputSignal);
FFTn = length(S(:,1));
k = length(S(1,:));
N = FFTn;
W = exp(-1 * 2j * pi * (0:N-1) / N);
OutputSignal = zeros(FFTn, k);
p = nextpow2(FFTn);
for i = 1:k
    s = bitrevorder(S(:,i));
    s = [s zeros(1, (2^p) - length(s))];
    WINDOWSIZE_LOG = log2(N);

    % Decimation-In-Time (DIT) FFT Algorithm
    Half = 1;
    for step = 1:WINDOWSIZE_LOG
        for index = 0:(N / (2^(WINDOWSIZE_LOG - step))):(N - 1)
            for n = 0:Half - 1
                pos = n + index + 1;
                pow = (2^(WINDOWSIZE_LOG - step)) * n;
                w = W(pow + 1);
                a = (s(pos) + s(pos + Half) * w);
                b = (s(pos) - s(pos + Half) * w);
                s(pos) = a;
                s(pos + Half) = b;
            end
        end
        Half = Half * 2;
    end
    OutputSignal(:, i) = s;
end
end
```
Some critical aspects of the previous code are highlighted in the following:

* We use `W = exp(-1 * 2j * pi * (0:N-1) / N)` to compute the FFT rotation factors.  
  
* A layered iterative approach is used to perform the base 2 DIT FFT butterfly computation, doubling the data block size at each layer of the butterfly operation. Then update the data layer by layer and compute the butterfly structure using the rotation factor `W`.

* The FFT transformed data is deposited into `OutputSignal`, where each column corresponds to one FFT column of the input matrix.


### Downconversion

Each row of spectral components is downconverted to baseband through multiplication with the complex exponential:

$$
e^{-\frac{j2\pi kmL}{N'}} \quad (2)
$$

#### Index Definitions

- **m**: Row index, 0 <= m <= P-1
- **k**: Column index, 0 <= k <= N'-1

The **magnitude** of the exponential is unity over the array and the **phase** exhibits considerable variation.

The matlab code of the Downconversion is shown below:
```
E=zeros(Np,P);
a=zeros(Np,P);
for k = -Np/2:Np/2-1
    for input_index = 0:P-1
        a_value = -2 * pi * k * input_index * L / Np;
        a_value = mod(a_value + pi, 2 * pi) - pi;
        a_normalized = a_value / pi;
        if abs(a_normalized) < 0.00001
            a_normalized = 0;
        end
        a(k + Np/2 + 1, input_index + 1) = a_normalized;
        E(k + Np/2 + 1, input_index + 1) = exp(-1i * 2 * pi * k * input_index * L / Np);
    end
end
XD_beforeTranspose=XF1_shift.*E;
```
Some critical aspects of the previous code are highlighted in the following:

* We use `E(k + Np/2 + 1, input_index + 1) = exp(-1i * 2 * pi * k * input_index * L / Np);` to compute the complex exponential matrix `E` for down-conversion operations. The final result of this step is stored in the `XD_beforeTranspose` matrix.

### Conjugate Multiplication
In the cyclic spectral plane, each **cell** is computed by multiplying one **column** of the array with the **conjugate** of another column.  

- The **column separation** is determined by the **desired frequency separation** or **cycle frequency**:

$$
  \alpha = f_k - f_e \quad (3)
$$

- The **mid-point between the two columns** defines the **frequency bin of interest**:

$$
  f = \frac{f_k + f_e}{2} \quad (4)
$$

The matlab code of the Conjugate Multiplication is shown below:
```
XM=zeros(P,Np^2);
for k=1:Np
    for l=1:Np
        XM(:,(k-1)*Np+l)=(XD(:,k).*conj(XD(:,l)));  % Auto-correlation
    end
end
result.Multi = single(XM);
XM = single(XM);
```

### Second FFT (32-pt)
The product from the previous multiplication is 32-pt FFT to yield a P-point result. Only the middle of the resulting spectrum is retained and stored into the designated f,a cell.
The upper and lower ends are undesireable because of increased estimate variation at the channel pair ends.
The matlab code of the Second FFT is shown below:
```
XF2=FFTFloatv3_M1(XM);
XF2_1=fft(XM);
XF2=fftshift(XF2,1);
result.SecondFFT = single(XF2);
XF2 = single(XF2);
format long
XF2_2=XF2(P/4+1:3*P/4, :);  % Retain central portion after trimming
```

### Final Result Save 


The final result will be saved to the `data_out` folder, which contains 128 txt files, each containing 8192 complex numbers of SINGLE precision. In the second paragraph of test.mlx, we also wrote the code for comparing the results of the Matlab simulation with those of the VCK5000, and all the results can be considered to be identical at a precision of 0.001.


### Time Consume

The average run time we got after running the matlab script 100 times is 3677.267 ms, next we will get the run time of VCK5000 and compare it in the following steps.

|Model|Running Time(ms)|
| ------------- | ------------- |
|Matlab in x86|3677.267|





## References

* [Implementation of Cyclic Spectral Analysis Methods](https://apps.dtic.mil/sti/tr/pdf/ADA261715.pdf)


## Next Steps

After running the FAM on the Matlab, we are ready to move to the next module, [Module 02-AI Engine Design](../Module_02_aie).


