import java.util.ArrayList;
import java.util.List;

class Product {

  private int id;
  private String name;
  private String description;
  private double price;
  private int quantity;

  public Product(
    int id,
    String name,
    String description,
    double price,
    int quantity
  ) {
    this.id = id;
    this.name = name;
    this.description = description;
    this.price = price;
    this.quantity = quantity;
  }

  public String getName() {
    return name;
  }

  public int getId() {
    return id;
  }

  public double getPrice() {
    return price;
  }

  public String getDescription() {
    return description;
  }

  public int getQuantity() {
    return quantity;
  }
}

class ShoppingCart {

  private List<Product> items;

  public ShoppingCart() {
    this.items = new ArrayList<>();
  }

  public void addItem(Product product) {
    items.add(product);
  }

  public void removeItem(Product product) {
    items.remove(product);
  }

  public List<Product> getItems() {
    return items;
  }
}

class Customer {

  private int id;
  private String name;
  private String email;
  private String address;
  private ShoppingCart cart;

  public Customer(int id, String name, String email, String address) {
    this.id = id;
    this.name = name;
    this.email = email;
    this.address = address;
    this.cart = new ShoppingCart();
  }

  public String getName() {
    return name;
  }

  public int getId() {
    return id;
  }

  public String getEmail() {
    return email;
  }

  public String getAddress() {
    return address;
  }

  public List<Product> getCartItems() {
    return cart.getItems();
  }

  public void emptyCart() {
    cart.getItems().clear();
  }

  public void addToCart(Product p){
    this.cart.addItem(p);
  }
}

class Order {

  private int orderId;
  private Customer customer;
  private List<Product> customerItems;

  public Order(int orderId, Customer customer) {
    this.orderId = orderId;
    this.customer = customer;
    this.customerItems = this.customer.getCartItems();
  }

  public int getId() {
    return orderId;
  }

  public String getName() {
    return customer.getName();
  }

  public List<Product> getCustomerItems() {
    return customerItems;
  }
}

class OnlineStore {

  private List<Product> products;
  private List<Customer> customers;
  private List<Order> orders;
  private int orderIdCounter;
  private int customerIdCounter;
  private int productIdCounter;
  private Customer loggedInCustomer;

  public OnlineStore() {
    System.out.print(
      "\n\tWELCOME TO OUR STORE Enjoy Your Stay !!\n"
    );
    this.products = new ArrayList<>();
    this.customers = new ArrayList<>();
    this.orders = new ArrayList<>();
    this.orderIdCounter = 1;
    this.customerIdCounter = 1;
    this.customerIdCounter = 1;
    // create ADMIN user here itself
    Customer ADMIN = new Customer(
      10101,
      "ADMIN",
      "ADMIN@store",
      "Store Headquarters, 5th Floor"
    );
    this.customers.add(ADMIN);
  }

  public void addProduct(String name, String desc, double price, int quant) {
    if (
      loggedInCustomer != null &&
      loggedInCustomer.getEmail().equals("ADMIN@store")
    ) {
        Product product = new Product(customerIdCounter++,name,desc,price,quant );
      products.add(product);
    }
  }

  public boolean delProduct(int id) {
    if (
      loggedInCustomer != null &&
      loggedInCustomer.getEmail().equals("ADMIN@store")
    ) {
        for (int i = 0; i < products.size(); i++) {
            if (products.get(i).getId() == id) {
                products.remove(i);
                return true;
            }
        }
        return false;
    }
    return false;
  }

  public void addCustomer(String name, String email, String address) {
    if (
      loggedInCustomer != null &&
      loggedInCustomer.getEmail().equals("ADMIN@store")
    ) {
        Customer customer = new Customer(productIdCounter++,name,email,address);
      customers.add(customer);
    }
  }

  public boolean delCustomer(int id) {
    if (
      loggedInCustomer != null &&
      loggedInCustomer.getEmail().equals("ADMIN@store")
    ) {
        for (int i = 0; i < customers.size(); i++) {
            if (customers.get(i).getId() == id) {
                customers.remove(i);
                return true;
            }
        }
        return false;
    }
    return false;
  }

  public void searchProducts(String keyword) {
    System.out.print("\n\tSEARCHING FOR "+keyword);
    List<Product> matchingProducts = new ArrayList<>();
    for (Product product : products) {
      if (product.getName().toLowerCase().contains(keyword.toLowerCase())) {
        matchingProducts.add(product);
      }
    }
    for(Product p: matchingProducts){
      System.out.print("\n\t" + p.getName()+" "+p.getDescription());
    }
  }

  public static String padLeft(String s, int n) {
    return String.format("%" + n + "s", s);
  }

  public boolean login(String emailAddress) {
    for (Customer customer : customers) {
      if (customer.getEmail().equals(emailAddress)) {
        loggedInCustomer = customer;
        System.out.print(
            "\n\tWELCOME " + customer.getName()
          );
        return true;
      }
    }
    return false;
  }

  public void logout() {
    if(loggedInCustomer!=null)
    {System.out.print(
      "\n\tBYE " + loggedInCustomer.getName()
    );}
    loggedInCustomer = null;
  }

  public void placeOrder() {
    if (loggedInCustomer == null) {
      System.out.print(
        "\n\tYOU ARE LOGGED OUT (ORDER NOT PLACED)"
      );
      return;
    }
    System.out.print(
      "\n\tPLACING YOUR ORDERS AND EMPTYING CART"
    );
    Order order = new Order(orderIdCounter++, loggedInCustomer);
    orders.add(order);
    loggedInCustomer.emptyCart();
  }

  public void showMyCart() {
    if (loggedInCustomer != null) {
      System.out.print(
        "\n\tSHOWING YOUR ORDERS " + loggedInCustomer.getName()+"\n"
      );
      for (Product p : loggedInCustomer.getCartItems()) {
        System.out.print("\n\t\t" + p.getName()+" "+p.getDescription());
      }
    } else {
      System.out.println(
        "\n\tYOU ARE LOGGED OUT (NO CART)\n"
      );
    }
  }

  public void showOrderqueue() {
    if (
      loggedInCustomer != null &&
      loggedInCustomer.getEmail().equals("ADMIN@store")
    ) {
      System.out.println(
        "\n\tSHOWING PENDING ORDERS"
      );
      for (Order O : orders) {
        System.out.print("\n\t\torder Id="+O.getId());
        System.out.print("\n\t\tcustomer Name="+O.getName());
        List<Product> customerItems = O.getCustomerItems();
        for (Product p : customerItems) {
          System.out.print("\n\t\t\t"+p.getName());
        }
      }
    }
  }

  public void addToMyCart(int id){
    if (loggedInCustomer != null) {
      for (Product p : products) {
        if(p.getId()==id){
          System.out.println(
            "\n\tADDING "+ p.getName() + " TO CART"
          );
          loggedInCustomer.addToCart(p);
          return;
        }
      }
    } else {
      System.out.println(
        "\n\tYOU ARE LOGGED OUT"
      );
    }
  }
}

public class shopping {

  public static void main(String[] args) {

    //create store object (ADMIN user automatically created)
    OnlineStore S = new OnlineStore();

    //login as ADMIN, add customers, add products
    S.login("ADMIN@store");
    S.addCustomer("anveshak", "anveshak@gmail", "kanpur");
    S.addCustomer("khyati", "khyati@gmail", "bangalore");
    S.addCustomer("tejas", "tejas@gmail", "jagdishpur");
    S.addCustomer("nitya", "nitya@gmail", "banaras");
    S.addProduct("ASUS Vivobook 14 Laptop","500Gb SSD, AMD Ryzen 7",49000.0,10);
    S.addProduct("HP Pavillion 15 Laptop","256Gb HDD, Intel i5",65000.0,30);
    S.addProduct("Dog Food","Petpure for small dog",190,20);
    S.addProduct("Banana Journal Diary","for note taking",75.6,60);
    S.addProduct("Zen Plant","cute indoor plant",500,5);
    S.logout(); //logout admin

    //login a normal user, placeorder
    S.placeOrder();
    S.showMyCart();
    S.login("khyati@gmail");
    S.searchProducts("laptop");
    S.addToMyCart(2);
    S.addToMyCart(3);
    S.addToMyCart(4);
    S.placeOrder();
    S.logout();

    S.login("tejas@gmail");
    S.searchProducts("dog");
    S.addToMyCart(4);
    S.placeOrder();
    S.logout();

    //print all orders as ADMIN
    S.login("ADMIN@store");
    S.showOrderqueue();
    S.logout();
  }
}
