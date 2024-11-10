struct RSA{
    n:i128,
    phin: i128
}

impl RSA{
    fn new(p:i128,q:i128)->Self{
        if !Self::is_prime(p) || !Self::is_prime(q){
            panic!("Both numbers must be prime");
        }
        RSA{
            n:p*q,
            phin:(p-1)*(q-1)
        }
    }

    fn is_prime(num:i128) -> bool{
        if num <= 1 {
            return false;
        }
        if num == 2 {
            return true;
        }
        if num % 2 == 0 {
            return false;
        }
        let mut i = 3;
        while i*i <= num/2 {
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

    fn encrypt(&self, message: i128, e: i128) -> i128{
        let mut result = 1;
        for _ in 0..e{
            result = (result * message) % self.n;
        }
        result
    }

    fn decrypt(&self, message: i128, d: i128) -> i128{
        let mut result = 1;
        for _ in 0..d{
            result = (result * message) % self.n;
        }
        result
    }
}

fn main() {
    println!("Enter two prime numbers:");

    let mut p = String::new();
    let mut q = String::new();

    std::io::stdin()
    .read_line(&mut p)
    .expect("Failed to read input");

    std::io::stdin()
    .read_line(&mut q)
    .expect("Failed to read input");

    let p = match p.trim().parse::<i128>(){
        Ok(num) => num,
        Err(_) => panic!("Invalid input")
    };

    let q = match q.trim().parse::<i128>(){
        Ok(num) => num,
        Err(_) => panic!("Invalid input")
    };

    let rsa = RSA::new(p, q);

    println!("Enter a number e:");

    let mut e = String::new();

    std::io::stdin()
    .read_line(&mut e)
    .expect("Failed to read input");

    let e = match e.trim().parse::<i128>(){
        Ok(num) => num,
        Err(_) => panic!("Invalid input")
    };

    
    let d = rsa.generate_key(e);

    println!("d: {}", d);

    println!("Enter a message:");

    let mut message = String::new();
    
    std::io::stdin()
    .read_line(&mut message)
    .expect("Failed to read input");

    let message = match message.trim().parse::<i128>(){
        Ok(num) => num,
        Err(_) => panic!("Invalid input")
    };

    let encrypted_message = rsa.encrypt(message, e);
    println!("Encrypted message: {}", encrypted_message);

    let decrypted_message = rsa.decrypt(encrypted_message, d);
    println!("Decrypted message: {}", decrypted_message);
}