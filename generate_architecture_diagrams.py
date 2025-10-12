#!/usr/bin/env python3
"""
Tourmate System Architecture Diagram Generator
This script creates visual representations of the system architecture
using matplotlib and other visualization libraries.
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyBboxPatch, ConnectionPatch
import numpy as np

def create_layered_architecture_diagram():
    """Create a layered architecture diagram for the Tourmate system."""
    
    fig, ax = plt.subplots(1, 1, figsize=(14, 10))
    
    # Define colors
    colors = {
        'presentation': '#E3F2FD',
        'business': '#F3E5F5',
        'data': '#E8F5E8',
        'storage': '#FFF3E0',
        'border': '#424242'
    }
    
    # Layer coordinates
    layers = [
        {'name': 'Presentation Layer\n(Console Interface)', 'y': 8, 'color': colors['presentation']},
        {'name': 'Business Logic Layer\n(Core Modules)', 'y': 6, 'color': colors['business']},
        {'name': 'Data Access Layer\n(File Operations)', 'y': 4, 'color': colors['data']},
        {'name': 'Storage Layer\n(CSV Files)', 'y': 2, 'color': colors['storage']}
    ]
    
    # Draw layers
    layer_height = 1.5
    layer_width = 12
    x_start = 1
    
    for i, layer in enumerate(layers):
        # Main layer box
        rect = FancyBboxPatch(
            (x_start, layer['y']), layer_width, layer_height,
            boxstyle="round,pad=0.1",
            facecolor=layer['color'],
            edgecolor=colors['border'],
            linewidth=2
        )
        ax.add_patch(rect)
        
        # Layer label
        ax.text(x_start + layer_width/2, layer['y'] + layer_height/2, 
                layer['name'], 
                ha='center', va='center', 
                fontsize=12, fontweight='bold')
    
    # Add components to each layer
    components = {
        'Presentation': ['Menu System', 'Input/Output Handlers', 'User Interface'],
        'Business': ['Auth Manager', 'Vehicle Manager', 'Sales Manager', 'Company Manager'],
        'Data Access': ['File I/O Manager', 'Data Parser', 'Backup Manager', 'Error Handler'],
        'Storage': ['vehicles.csv', 'sales.csv', 'users.csv', 'company.csv']
    }
    
    # Add component boxes
    component_positions = {
        'Presentation': 8.2,
        'Business': 6.2,
        'Data Access': 4.2,
        'Storage': 2.2
    }
    
    for layer_name, comps in components.items():
        y_pos = component_positions[layer_name]
        comp_width = layer_width / len(comps) - 0.2
        
        for i, comp in enumerate(comps):
            x_pos = x_start + 0.1 + i * (comp_width + 0.2)
            
            comp_rect = FancyBboxPatch(
                (x_pos, y_pos), comp_width, 0.6,
                boxstyle="round,pad=0.05",
                facecolor='white',
                edgecolor=colors['border'],
                linewidth=1
            )
            ax.add_patch(comp_rect)
            
            ax.text(x_pos + comp_width/2, y_pos + 0.3, 
                    comp, 
                    ha='center', va='center', 
                    fontsize=9, fontweight='normal')
    
    # Add arrows between layers
    arrow_props = dict(arrowstyle='->', lw=2, color=colors['border'])
    
    for i in range(len(layers) - 1):
        y_start = layers[i]['y']
        y_end = layers[i+1]['y'] + layer_height
        
        for j in range(3):  # Multiple arrows
            x_pos = x_start + 2 + j * 4
            ax.annotate('', xy=(x_pos, y_end), xytext=(x_pos, y_start),
                       arrowprops=arrow_props)
    
    # Set title and formatting
    ax.set_title('Tourmate Vehicle Management System\nLayered Architecture', 
                fontsize=16, fontweight='bold', pad=20)
    
    ax.set_xlim(0, 14)
    ax.set_ylim(1, 11)
    ax.set_aspect('equal')
    ax.axis('off')
    
    plt.tight_layout()
    plt.savefig('c:/F-DRIVE/GIT/yuity/layered_architecture.png', 
                dpi=300, bbox_inches='tight')
    plt.show()

def create_module_interaction_diagram():
    """Create a module interaction diagram showing relationships between modules."""
    
    fig, ax = plt.subplots(1, 1, figsize=(16, 12))
    
    # Define module positions
    modules = {
        'Main': {'pos': (8, 10), 'color': '#FF6B6B'},
        'MenuManager': {'pos': (8, 8), 'color': '#4ECDC4'},
        'AuthManager': {'pos': (3, 6), 'color': '#45B7D1'},
        'VehicleManager': {'pos': (6, 6), 'color': '#96CEB4'},
        'SalesManager': {'pos': (10, 6), 'color': '#FECA57'},
        'CompanyManager': {'pos': (13, 6), 'color': '#FF9FF3'},
        'FileManager': {'pos': (8, 4), 'color': '#54A0FF'},
        'InputValidator': {'pos': (4, 2), 'color': '#5F27CD'},
        'ErrorHandler': {'pos': (12, 2), 'color': '#FF3838'}
    }
    
    # Draw modules
    for name, info in modules.items():
        circle = plt.Circle(info['pos'], 0.8, 
                          facecolor=info['color'], 
                          edgecolor='black', 
                          linewidth=2)
        ax.add_patch(circle)
        
        ax.text(info['pos'][0], info['pos'][1], name, 
                ha='center', va='center', 
                fontsize=10, fontweight='bold', 
                color='white')
    
    # Define connections
    connections = [
        ('Main', 'MenuManager'),
        ('MenuManager', 'AuthManager'),
        ('MenuManager', 'VehicleManager'),
        ('MenuManager', 'SalesManager'),
        ('MenuManager', 'CompanyManager'),
        ('VehicleManager', 'FileManager'),
        ('SalesManager', 'FileManager'),
        ('AuthManager', 'FileManager'),
        ('CompanyManager', 'FileManager'),
        ('VehicleManager', 'InputValidator'),
        ('SalesManager', 'InputValidator'),
        ('AuthManager', 'InputValidator'),
        ('FileManager', 'ErrorHandler'),
        ('InputValidator', 'ErrorHandler')
    ]
    
    # Draw connections
    for start, end in connections:
        start_pos = modules[start]['pos']
        end_pos = modules[end]['pos']
        
        ax.annotate('', xy=end_pos, xytext=start_pos,
                   arrowprops=dict(arrowstyle='->', lw=2, color='gray'))
    
    # Add legend
    legend_elements = []
    for name, info in modules.items():
        legend_elements.append(plt.Line2D([0], [0], marker='o', color='w', 
                                        markerfacecolor=info['color'], 
                                        markersize=10, label=name))
    
    ax.legend(handles=legend_elements, loc='upper left', bbox_to_anchor=(0, 1))
    
    ax.set_title('Tourmate System Module Interaction Diagram', 
                fontsize=16, fontweight='bold')
    
    ax.set_xlim(0, 16)
    ax.set_ylim(0, 12)
    ax.set_aspect('equal')
    ax.axis('off')
    
    plt.tight_layout()
    plt.savefig('c:/F-DRIVE/GIT/yuity/module_interaction.png', 
                dpi=300, bbox_inches='tight')
    plt.show()

def create_data_flow_diagram():
    """Create a data flow diagram showing how data moves through the system."""
    
    fig, ax = plt.subplots(1, 1, figsize=(14, 10))
    
    # Define processes (circles)
    processes = {
        'User Input': {'pos': (2, 8), 'color': '#FF6B6B'},
        'Validate Input': {'pos': (5, 8), 'color': '#4ECDC4'},
        'Process Request': {'pos': (8, 8), 'color': '#45B7D1'},
        'Update Data': {'pos': (11, 8), 'color': '#96CEB4'},
        'Generate Output': {'pos': (8, 5), 'color': '#FECA57'},
        'Display Result': {'pos': (5, 5), 'color': '#FF9FF3'}
    }
    
    # Define data stores (rectangles)
    data_stores = {
        'Vehicle Files': {'pos': (11, 5), 'size': (2, 0.8), 'color': '#E8F5E8'},
        'Sales Files': {'pos': (11, 3.5), 'size': (2, 0.8), 'color': '#E8F5E8'},
        'User Files': {'pos': (11, 2), 'size': (2, 0.8), 'color': '#E8F5E8'},
        'Backup Files': {'pos': (8, 2), 'size': (2, 0.8), 'color': '#FFF3E0'}
    }
    
    # Define external entities (squares)
    external_entities = {
        'System User': {'pos': (2, 5), 'size': (1.5, 1), 'color': '#FFE4B5'},
        'Administrator': {'pos': (2, 3), 'size': (1.5, 1), 'color': '#FFE4B5'}
    }
    
    # Draw processes
    for name, info in processes.items():
        circle = plt.Circle(info['pos'], 0.8, 
                          facecolor=info['color'], 
                          edgecolor='black', 
                          linewidth=2)
        ax.add_patch(circle)
        
        ax.text(info['pos'][0], info['pos'][1], name, 
                ha='center', va='center', 
                fontsize=9, fontweight='bold')
    
    # Draw data stores
    for name, info in data_stores.items():
        rect = FancyBboxPatch(
            (info['pos'][0] - info['size'][0]/2, info['pos'][1] - info['size'][1]/2),
            info['size'][0], info['size'][1],
            boxstyle="round,pad=0.1",
            facecolor=info['color'],
            edgecolor='black',
            linewidth=2
        )
        ax.add_patch(rect)
        
        ax.text(info['pos'][0], info['pos'][1], name, 
                ha='center', va='center', 
                fontsize=9, fontweight='bold')
    
    # Draw external entities
    for name, info in external_entities.items():
        rect = FancyBboxPatch(
            (info['pos'][0] - info['size'][0]/2, info['pos'][1] - info['size'][1]/2),
            info['size'][0], info['size'][1],
            boxstyle="square,pad=0.1",
            facecolor=info['color'],
            edgecolor='black',
            linewidth=2
        )
        ax.add_patch(rect)
        
        ax.text(info['pos'][0], info['pos'][1], name, 
                ha='center', va='center', 
                fontsize=9, fontweight='bold')
    
    # Define data flows with labels
    data_flows = [
        ('System User', 'User Input', 'Menu Selection'),
        ('User Input', 'Validate Input', 'Raw Input'),
        ('Validate Input', 'Process Request', 'Valid Data'),
        ('Process Request', 'Update Data', 'Business Logic'),
        ('Update Data', 'Vehicle Files', 'Vehicle Data'),
        ('Update Data', 'Sales Files', 'Sales Data'),
        ('Update Data', 'User Files', 'User Data'),
        ('Update Data', 'Generate Output', 'Updated Data'),
        ('Generate Output', 'Display Result', 'Formatted Output'),
        ('Display Result', 'System User', 'System Response'),
        ('Vehicle Files', 'Backup Files', 'Backup Data'),
        ('Administrator', 'Process Request', 'Admin Commands')
    ]
    
    # Draw data flows
    for start, end, label in data_flows:
        if start in processes:
            start_pos = processes[start]['pos']
        elif start in data_stores:
            start_pos = data_stores[start]['pos']
        else:
            start_pos = external_entities[start]['pos']
        
        if end in processes:
            end_pos = processes[end]['pos']
        elif end in data_stores:
            end_pos = data_stores[end]['pos']
        else:
            end_pos = external_entities[end]['pos']
        
        ax.annotate('', xy=end_pos, xytext=start_pos,
                   arrowprops=dict(arrowstyle='->', lw=1.5, color='blue'))
        
        # Add label to flow
        mid_x = (start_pos[0] + end_pos[0]) / 2
        mid_y = (start_pos[1] + end_pos[1]) / 2
        ax.text(mid_x, mid_y + 0.2, label, 
                ha='center', va='center', 
                fontsize=7, color='blue',
                bbox=dict(boxstyle="round,pad=0.3", facecolor='white', alpha=0.8))
    
    ax.set_title('Tourmate System Data Flow Diagram', 
                fontsize=16, fontweight='bold')
    
    ax.set_xlim(0, 14)
    ax.set_ylim(0, 10)
    ax.set_aspect('equal')
    ax.axis('off')
    
    plt.tight_layout()
    plt.savefig('c:/F-DRIVE/GIT/yuity/data_flow_diagram.png', 
                dpi=300, bbox_inches='tight')
    plt.show()

def create_console_interface_mockup():
    """Create a mockup of the console interface design."""
    
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))
    
    # Console color scheme
    console_bg = '#2E3440'
    text_color = '#D8DEE9'
    highlight_color = '#88C0D0'
    
    # Main Menu Design
    ax1.set_facecolor(console_bg)
    main_menu_text = """
======================================
    TOUR MATE MANAGEMENT SYSTEM
======================================
1. Vehicle Management
2. Sales Management  
3. Company Information
4. User Management
5. System Backup
6. Exit System
======================================
Enter your choice (1-6): _
"""
    ax1.text(0.05, 0.95, main_menu_text, 
            transform=ax1.transAxes, 
            fontfamily='monospace', fontsize=10,
            color=text_color, va='top')
    ax1.set_title('Main Menu Interface', color=highlight_color, fontweight='bold')
    ax1.axis('off')
    
    # Vehicle Management Menu
    ax2.set_facecolor(console_bg)
    vehicle_menu_text = """
======================================
      VEHICLE MANAGEMENT
======================================
1. View All Vehicles
2. Add New Vehicle
3. Update Vehicle Details
4. Delete Vehicle
5. Search Vehicles
6. Back to Main Menu
======================================
Enter your choice (1-6): _
"""
    ax2.text(0.05, 0.95, vehicle_menu_text, 
            transform=ax2.transAxes, 
            fontfamily='monospace', fontsize=10,
            color=text_color, va='top')
    ax2.set_title('Vehicle Management Menu', color=highlight_color, fontweight='bold')
    ax2.axis('off')
    
    # Vehicle List Display
    ax3.set_facecolor(console_bg)
    vehicle_list_text = """
======================================
         VEHICLE LIST
======================================
ID | License   | Model        | Status
---|-----------|--------------|----------
1  | ABC-123   | Toyota Camry | Available
2  | XYZ-456   | Honda Civic  | In-Service
3  | DEF-789   | Ford Focus   | Maintenance
4  | GHI-012   | Hyundai Elan | Available
======================================
Total Vehicles: 4
Press any key to continue...
"""
    ax3.text(0.05, 0.95, vehicle_list_text, 
            transform=ax3.transAxes, 
            fontfamily='monospace', fontsize=9,
            color=text_color, va='top')
    ax3.set_title('Vehicle List Display', color=highlight_color, fontweight='bold')
    ax3.axis('off')
    
    # Add Vehicle Form
    ax4.set_facecolor(console_bg)
    add_vehicle_text = """
======================================
        ADD NEW VEHICLE
======================================
License Plate: ABC-999
Vehicle Model: Toyota Corolla
Vehicle Year: 2023
Vehicle Type: Sedan
Seating Capacity: 5
Current Status: Available
Registration Date: 2024-10-11

======================================
Confirm addition? (Y/N): _
"""
    ax4.text(0.05, 0.95, add_vehicle_text, 
            transform=ax4.transAxes, 
            fontfamily='monospace', fontsize=9,
            color=text_color, va='top')
    ax4.set_title('Add Vehicle Form', color=highlight_color, fontweight='bold')
    ax4.axis('off')
    
    plt.suptitle('Console Interface Design Mockups', 
                fontsize=16, fontweight='bold', color='black')
    plt.tight_layout()
    plt.savefig('c:/F-DRIVE/GIT/yuity/console_interface_mockup.png', 
                dpi=300, bbox_inches='tight', facecolor='white')
    plt.show()

def main():
    """Generate all architecture diagrams."""
    
    print("Generating Tourmate System Architecture Diagrams...")
    
    try:
        print("1. Creating Layered Architecture Diagram...")
        create_layered_architecture_diagram()
        
        print("2. Creating Module Interaction Diagram...")
        create_module_interaction_diagram()
        
        print("3. Creating Data Flow Diagram...")
        create_data_flow_diagram()
        
        print("4. Creating Console Interface Mockup...")
        create_console_interface_mockup()
        
        print("\nAll diagrams generated successfully!")
        print("Files saved:")
        print("- layered_architecture.png")
        print("- module_interaction.png")
        print("- data_flow_diagram.png")
        print("- console_interface_mockup.png")
        
    except Exception as e:
        print(f"Error generating diagrams: {e}")
        print("Make sure matplotlib is installed: pip install matplotlib")

if __name__ == "__main__":
    main()