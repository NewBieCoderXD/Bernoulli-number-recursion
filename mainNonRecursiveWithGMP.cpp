#include<iostream>
#include<vector>
#include<cmath>
#include<gmp.h>
#include<gmpxx.h>

typedef long long ll;

// ll gcd(ll a,ll b){
//     a=llabs(a);
//     b=llabs(b);
//     // std::cout<<a<<" "<<b<<"\n";
//     if(a>b){
//         std::swap(a,b);
//     }
//     if(a==0){
//         return b;
//     }
//     return gcd(b%a,a);
// }

// ll lcm(ll a, ll b){
//     return a*b/gcd(a,b);
// }

struct Fraction{
    mpz_class numerator,denominator;
    template <typename T>
    Fraction(T numerator,T denominator):numerator(numerator),denominator(denominator){
        // numerator=inNumerator;
        // denominator=inDenominator;
        // mpz_init_set_str(numerator,inNumerator,10);
        // mpz_init_set_str(denominator,inDenominator,10);
    }

    Fraction toProper(){
        if(numerator==0){
            denominator=1;
            return *this;
        }
        mpz_class fractionalGCD = gcd(numerator,denominator);
        numerator/=fractionalGCD;
        denominator/=fractionalGCD;
        return *this;
    }

    Fraction operator+(Fraction frac){
        mpz_class denominatorLCM = lcm(frac.denominator,this->denominator);
        return Fraction(
            mpz_class(denominatorLCM/denominator*numerator+denominatorLCM/frac.denominator*frac.numerator),
            denominatorLCM
        );
    }

    Fraction operator-(Fraction frac){
        mpz_class denominatorLCM = lcm(frac.denominator,this->denominator);
        // std::cout<<"gg "<<lcm/this->denominator*this->numerator<<" "<<lcm/frac.denominator*frac.numerator<<'\n';
        // std::cout<<"gggg "<<Fraction(lcm/this->denominator*this->numerator-lcm/frac.denominator*frac.numerator,lcm)<<"\n";
        return Fraction(
            mpz_class(denominatorLCM/this->denominator*this->numerator-denominatorLCM/frac.denominator*frac.numerator),
            denominatorLCM
        );
    }

    Fraction operator*(mpz_class multi){
        return Fraction(mpz_class(this->numerator*multi),this->denominator).toProper();
    }

    void operator+=(Fraction frac){
        *this=operator+(frac);
    }

    void operator-=(Fraction frac){
        *this=operator-(frac);
    }

    // Fraction operator/(Fraction frac){
    //     return 
    // }

    Fraction operator/(mpz_class divisor){
        return Fraction(this->numerator,mpz_class(this->denominator*divisor)).toProper();
    }

    friend std::ostream& operator<<(std::ostream& stream, const Fraction& frac){
        stream<<frac.numerator<<"/"<<frac.denominator;
        return stream;
    }
};

mpz_class C(std::vector<std::vector<mpz_class>>& memoizC,int n,int k){
    k=std::min(k,n-k);
    return memoizC[n][k];
}

void fillC(std::vector<std::vector<mpz_class>>& memoizC,int n){
    // std::cout<<"C n: "<<n<<" k: "<<k<<'\n';
    for(int i=1;i<=n;i++){
        memoizC[i][0]=1;
        for(int j=1;j<memoizC[i].size();j++){
            memoizC[i][j]=C(memoizC,i-1,j-1)+C(memoizC,i-1,j);
        }
    }
    // memoizC[n][0]=1;
    // // std::cout<<std::min(k,n-1-k)<<" "<<std::min(k-1,n-k)<<"\n";
    // memoizC[n][k]=memoizC[n-1][std::min(k,n-1-k)]+memoizC[n-1][std::min(k-1,n-k)];
    // return memoizC[n][k];
}



Fraction B(std::vector<Fraction>& memoizB,std::vector<std::vector<mpz_class>>& memoizC,int n){
    if(memoizB[n].denominator!=0){
        // std::cout<<"n "<<n<<" memoizB: "<<memoizB[n]<<"\n";
        return memoizB[n];
    }
    // std::cout<<"n of B: "<<n<<"\n";
    if(n%2!=0&&n>=3){
        return Fraction(0,1);
    }
    for(int i=1;i<=n;i++){
        if(i%2!=0&&i>3){
            memoizB[i]=Fraction(0,1);
            continue;
        }
        Fraction answer(0,1);
        for(int j=0;j<i;j++){
            // std::cout<<"n: "<<n<<" i: "<<i<<'\n';
            Fraction b=memoizB[j];
            if(b.numerator==0){
                continue;
            }
            // std::cout<<"C: "<<i+1<<" "<<j<<" value: "<<C(memoizC,i+1,j)<<"\n";
            answer-=b*mpz_class(C(memoizC,i+1,j));
        }
        memoizB[i]=(answer/(i+1)).toProper();
    }
    return memoizB[n];
}

int main(int argc, char **argv){
    int n;
    if(argc==1){
        std::cin>>n;
        // n=6;
    }
    else{
        n=std::strtol(argv[1],nullptr,10);
    }
    std::vector<std::vector<mpz_class>> memoizC(n+2);
    std::vector<Fraction> memoizB(n+1,Fraction(0,0));
    for(int i=0;i<memoizC.size();i++){
        // std::cout<<"n: "<<i<<" size: "<<ceil(i/2.0)+(i%2==0?1:0)<<"\n";
        memoizC[i]=std::vector<mpz_class>(ceil(i/2.0)+(i%2==0?1:0),0);
    }
    memoizC[1][0]=1;
    memoizB[0]=Fraction(1,1);
    fillC(memoizC,n+1);
    std::cout<<B(memoizB,memoizC,n)<<"\n";
    // for(int i=0;i<memoizB.size();i++){
    //     std::cout<<"n: "<<i<<" memoizB: "<<memoizB[i]<<"\n";
    // }
}