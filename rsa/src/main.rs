use std::io::{self};

struct RSA {
    n: i128,
    phin: i128,
}

impl RSA {
    fn new(p: i128, q: i128) -> Self {
        if !Self::is_prime(p) || !Self::is_prime(q) {
            panic!("Both numbers must be prime");
        }
        RSA {
            n: p * q,
            phin: (p - 1) * (q - 1),
        }
    }

    fn is_prime(num: i128) -> bool {
        if num <= 1 {
            return false;
        }
        if num % 2 == 0 {
            return false;
        }
        let mut i = 3;
        while i * i <= num {
            if num % i == 0 {
                return false;
            }
            i += 2;
        }
        true
    }

    fn generate_key(&self, e: i128) -> i128{
        let mut a = self.phin;
        let mut b = e;

        while b != 0 {
            let temp = b;
            b = a % b;
            a = temp;
        }

        if a != 1 {
            panic!("e must be coprime with n");
        }


        let mut a = self.phin;
        let mut b = e;
        let mut x0 = 1;
        let mut x1 = 0;
        let mut y0 = 0;
        let mut y1 = 1;
        while b != 0 {
            let q = a / b;
            let temp = b;
            b = a % b;
            a = temp;
            let temp = x1;
            x1 = x0 - q * x1;
            x0 = temp;
            let temp = y1;
            y1 = y0 - q * y1;
            y0 = temp;
        }
        y0 = if y0 < 0 {self.phin + y0} else {y0};
        y0
    }

    fn encrypt(&self, message: &str, e: i128) -> Vec<i128> {
        message
            .bytes()
            .map(|byte| {
                let mut result = 1;
                let m = byte as i128;
                for _ in 0..e {
                    result = (result * m) % self.n;
                }
                result
            })
            .collect()
    }

    fn decrypt(&self, encrypted_message: Vec<i128>, d: i128) -> String {
        encrypted_message
            .iter()
            .map(|&c| {
                let mut result = 1;
                for _ in 0..d {
                    result = (result * c) % self.n;
                }
                result as u8 as char
            })
            .collect()
    }
}

fn main() {
    println!("Enter two prime numbers:");

    let mut input = String::new();
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let p: i128 = input.trim().parse().expect("Invalid input");

    input.clear();
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let q: i128 = input.trim().parse().expect("Invalid input");

    let rsa = RSA::new(p, q);

    println!("Enter a number e:");
    input.clear();
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let e: i128 = input.trim().parse().expect("Invalid input");

    let d = rsa.generate_key(e);

    println!("Public key: ({},{})", e, rsa.n);
    println!("Private key: ({},{})", d, rsa.n);

    println!("Enter a message:");
    input.clear();
    io::stdin().read_line(&mut input).expect("Failed to read input");
    let message = input.trim();

    let encrypted_message = rsa.encrypt(message, e);
    println!("Encrypted message: {:?}", encrypted_message);

    let decrypted_message = rsa.decrypt(encrypted_message, d);
    println!("Decrypted message: {:?}", decrypted_message);
}
