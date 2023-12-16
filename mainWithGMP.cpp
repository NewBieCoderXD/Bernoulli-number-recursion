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
        ).toProper();
    }

    Fraction operator-(Fraction frac){
        mpz_class denominatorLCM = lcm(frac.denominator,this->denominator);
        // std::cout<<"gg "<<lcm/this->denominator*this->numerator<<" "<<lcm/frac.denominator*frac.numerator<<'\n';
        // std::cout<<"gggg "<<Fraction(lcm/this->denominator*this->numerator-lcm/frac.denominator*frac.numerator,lcm)<<"\n";
        return Fraction(
            mpz_class(denominatorLCM/this->denominator*this->numerator-denominatorLCM/frac.denominator*frac.numerator),
            denominatorLCM
        ).toProper();
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
    // std::cout<<"C n: "<<n<<" k: "<<k<<'\n';
    if(k==0){
        return 1;
    }
    if(memoizC[n][k]!=0){
        return memoizC[n][k];
    }
    memoizC[n][k]=C(memoizC,n-1,k)+C(memoizC,n-1,k-1);
    return memoizC[n][k];
}

Fraction B(std::vector<Fraction>& memoizB,std::vector<std::vector<mpz_class>>& memoizC,int n){
    if(memoizB[n].denominator!=0){
        // std::cout<<"n "<<n<<" memoizB: "<<memoizB[n]<<"\n";
        return memoizB[n];
    }
    // std::cout<<"n of B: "<<n<<"\n";
    Fraction answer(0,1);
    for(ll i=0;i<n;i++){
        // std::cout<<"n: "<<n<<" i: "<<i<<'\n';
        answer-=B(memoizB,memoizC,i)*mpz_class(C(memoizC,n+1,i));
    }
    memoizB[n]=answer/(n+1);
    return memoizB[n];
}

int main(){
    int n;
    std::cin>>n;
    std::vector<std::vector<mpz_class>> memoizC(n+2);
    std::vector<Fraction> memoizB(n+1,Fraction(0,0));
    for(int i=0;i<memoizC.size();i++){
        // std::cout<<"n: "<<i<<" size: "<<ceil(i/2.0)+(i%2==0?1:0)<<"\n";
        memoizC[i]=std::vector<mpz_class>(ceil(i/2.0)+(i%2==0?1:0),0);
    }
    memoizC[1][0]=1;
    memoizB[0]=Fraction(1,1);
    // std::cout<<Fraction(1,2)<<"\n";
    // std::cout<<C(memoizC,4,2)<<"\n";
    std::cout<<B(memoizB,memoizC,n)<<"\n";
    // for(int i=0;i<memoizB.size();i++){
    //     std::cout<<"n: "<<i<<" memoizB: "<<memoizB[i]<<"\n";
    // }
}