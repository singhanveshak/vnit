import java.util.ArrayList;
import java.util.List;

interface MenuItem {
    String getName();
    String getDescription();
    double getPrice();
  }
  
   class RestaurantMenuItem implements MenuItem {
    private String name;
    private String description;
    private double price;
  
    public RestaurantMenuItem(String name, String description, double price) {
      this.name = name;
      this.description = description;
      this.price = price;
    }
  
    @Override
    public String getName() {
      return name;
    }
  
    @Override
    public String getDescription() {
      return description;
    }
  
    @Override
    public double getPrice() {
      return price;
    }
  }
  
   interface Order {
    void addItem(MenuItem item);
    void removeItem(MenuItem item);
    double getTotalPrice();
  }
  
   class RestaurantOrder implements Order {
    private List<MenuItem> items;
  
    public RestaurantOrder() {
      this.items = new ArrayList<>();
    }
  
    @Override
    public void addItem(MenuItem item) {
      items.add(item);
    }
  
    @Override
    public void removeItem(MenuItem item) {
      items.remove(item);
    }
  
    @Override
    public double getTotalPrice() {
      double totalPrice = 0;
      for (MenuItem item : items) {
        totalPrice += item.getPrice();
      }
      return totalPrice;
    }
    
    public void printOrder(){
      // Print order details
      System.out.println("Order Details:");
      for (MenuItem item : items) {
        System.out.println(String.format("- %s: %s ($%.2f)", item.getName(), item.getDescription(), item.getPrice()));
      }
      System.out.println(String.format("Total Price: $%.2f", getTotalPrice()));
    }  
  
}
  
   interface RestaurantSystem {
    RestaurantOrder createOrder();
    void addMenuItem(MenuItem item);
    void removeMenuItem(MenuItem item);
  }
  
   class RestaurantOrderSystem implements RestaurantSystem {
    public List<MenuItem> menuItems;
  
    public RestaurantOrderSystem() {
      this.menuItems = new ArrayList<>();
    }
  
    @Override
    public RestaurantOrder createOrder() {
      return new RestaurantOrder();
    }
  
    @Override
    public void addMenuItem(MenuItem item) {
      menuItems.add(item);
    }
  
    @Override
    public void removeMenuItem(MenuItem item) {
      menuItems.remove(item);
    }

  }
  
  public class Main {
    public static void main(String[] args) {
      RestaurantOrderSystem system = new RestaurantOrderSystem();
  
      // Add some menu items
      system.addMenuItem(new RestaurantMenuItem("Pizza", "Large cheese pizza", 12.99));
      system.addMenuItem(new RestaurantMenuItem("Burger", "Cheeseburger with fries", 9.99));
      system.addMenuItem(new RestaurantMenuItem("Salad", "Garden salad with balsamic dressing", 7.50));
  
      // Create an order
      RestaurantOrder order = system.createOrder();
  
      // Add items to the order
      order.addItem(system.menuItems.get(0));  // Pizza
      order.addItem(system.menuItems.get(1));  // Burger
  
      // Print order details
      order.printOrder();
    }
  }
  
