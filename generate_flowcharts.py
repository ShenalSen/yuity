#!/usr/bin/env python3
"""
Tourmate System Flowcharts Generator
This script creates professional flowcharts using matplotlib for the Tourmate system
demonstrating control structures and modularization.
"""

import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyBboxPatch, Circle, Polygon
import numpy as np

class FlowchartGenerator:
    def __init__(self):
        self.fig_width = 16
        self.fig_height = 12
        self.colors = {
            'terminal': '#FF6B6B',      # Start/End - Red
            'process': '#4ECDC4',       # Process - Teal
            'decision': '#FFE66D',      # Decision - Yellow
            'input_output': '#95E1D3',  # Input/Output - Light Green
            'connector': '#A8E6CF',     # Connector - Light Blue
            'predefined': '#FFB3BA',    # Predefined Process - Light Pink
            'arrow': '#2C3E50',         # Arrows - Dark Blue
            'text': '#2C3E50'           # Text - Dark Blue
        }

    def create_flowchart_symbol(self, ax, symbol_type, x, y, width, height, text, text_size=9):
        """Create a flowchart symbol with text."""
        
        if symbol_type == 'terminal':
            # Oval shape for start/end
            ellipse = patches.Ellipse((x, y), width, height, 
                                    facecolor=self.colors['terminal'], 
                                    edgecolor='black', linewidth=2)
            ax.add_patch(ellipse)
            
        elif symbol_type == 'process':
            # Rectangle for process
            rect = FancyBboxPatch((x - width/2, y - height/2), width, height,
                                boxstyle="round,pad=0.05",
                                facecolor=self.colors['process'],
                                edgecolor='black', linewidth=2)
            ax.add_patch(rect)
            
        elif symbol_type == 'decision':
            # Diamond for decision
            diamond_points = np.array([[x, y + height/2],    # Top
                                     [x + width/2, y],        # Right
                                     [x, y - height/2],       # Bottom
                                     [x - width/2, y]])       # Left
            diamond = Polygon(diamond_points, 
                            facecolor=self.colors['decision'],
                            edgecolor='black', linewidth=2)
            ax.add_patch(diamond)
            
        elif symbol_type == 'input_output':
            # Parallelogram for input/output
            para_points = np.array([[x - width/2 + 0.2, y + height/2],
                                  [x + width/2, y + height/2],
                                  [x + width/2 - 0.2, y - height/2],
                                  [x - width/2, y - height/2]])
            parallelogram = Polygon(para_points,
                                  facecolor=self.colors['input_output'],
                                  edgecolor='black', linewidth=2)
            ax.add_patch(parallelogram)
            
        elif symbol_type == 'predefined':
            # Rectangle with double lines for predefined process
            rect = FancyBboxPatch((x - width/2, y - height/2), width, height,
                                boxstyle="round,pad=0.05",
                                facecolor=self.colors['predefined'],
                                edgecolor='black', linewidth=2)
            ax.add_patch(rect)
            # Add inner lines
            inner_rect = FancyBboxPatch((x - width/2 + 0.1, y - height/2 + 0.1), 
                                      width - 0.2, height - 0.2,
                                      boxstyle="round,pad=0.05",
                                      facecolor='none',
                                      edgecolor='black', linewidth=1)
            ax.add_patch(inner_rect)
        
        # Add text
        ax.text(x, y, text, ha='center', va='center', 
                fontsize=text_size, fontweight='bold', 
                color=self.colors['text'], wrap=True)

    def draw_arrow(self, ax, start, end, label=None, label_pos='middle'):
        """Draw an arrow between two points with optional label."""
        ax.annotate('', xy=end, xytext=start,
                   arrowprops=dict(arrowstyle='->', lw=2, color=self.colors['arrow']))
        
        if label:
            if label_pos == 'middle':
                mid_x = (start[0] + end[0]) / 2
                mid_y = (start[1] + end[1]) / 2
            elif label_pos == 'start':
                mid_x = start[0] + 0.3
                mid_y = start[1]
            else:  # end
                mid_x = end[0] - 0.3
                mid_y = end[1]
                
            ax.text(mid_x, mid_y, label, ha='center', va='center',
                   fontsize=8, color=self.colors['arrow'],
                   bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.8))

    def create_main_system_flowchart(self):
        """Create the main system flowchart."""
        fig, ax = plt.subplots(1, 1, figsize=(self.fig_width, self.fig_height))
        
        # Define coordinates for symbols
        symbols = [
            ('terminal', 8, 11, 2, 0.8, 'START'),
            ('process', 8, 10, 2.5, 0.8, 'Initialize System'),
            ('process', 8, 9, 2.5, 0.8, 'Load Configuration'),
            ('input_output', 8, 8, 2.5, 0.8, 'Display Welcome Screen'),
            ('decision', 8, 7, 3, 1, 'Authentication\nRequired?'),
            ('predefined', 12, 7, 2.5, 0.8, 'Authentication\nModule'),
            ('input_output', 8, 5.5, 2.5, 0.8, 'Display Main Menu'),
            ('input_output', 8, 4.5, 2.5, 0.8, 'Get User Choice'),
            ('decision', 8, 3.5, 2.5, 1, 'Valid Choice?'),
            ('input_output', 4, 3.5, 2, 0.8, 'Display Error'),
            ('decision', 8, 2, 3, 1.2, 'Choice Analysis'),
            ('predefined', 3, 0.5, 2, 0.8, 'Vehicle Module'),
            ('predefined', 6, 0.5, 2, 0.8, 'Sales Module'),
            ('predefined', 10, 0.5, 2, 0.8, 'Company Module'),
            ('predefined', 13, 0.5, 2, 0.8, 'Exit Module'),
            ('terminal', 8, -1, 2, 0.8, 'END')
        ]
        
        # Create symbols
        for symbol_type, x, y, width, height, text in symbols:
            self.create_flowchart_symbol(ax, symbol_type, x, y, width, height, text)
        
        # Draw arrows with labels
        arrows = [
            ((8, 10.6), (8, 10.4), None),
            ((8, 9.6), (8, 9.4), None),
            ((8, 8.6), (8, 8.4), None),
            ((8, 7.6), (8, 7.4), None),
            ((9.5, 7), (12, 7), 'YES'),
            ((8, 6.5), (8, 6), 'NO'),
            ((12, 6.6), (8, 6), None),
            ((8, 5.1), (8, 4.9), None),
            ((8, 4.1), (8, 4), None),
            ((6.75, 3.5), (4, 3.5), 'NO'),
            ((4, 3.1), (4, 2.5), None),
            ((4, 2.5), (6.5, 2.5), None),
            ((8, 3), (8, 2.6), 'YES'),
            ((6.5, 2), (3, 1.3), 'Vehicle'),
            ((7, 2), (6, 1.3), 'Sales'),
            ((9, 2), (10, 1.3), 'Company'),
            ((9.5, 2), (13, 1.3), 'Exit'),
            ((8, 0.5), (8, -0.6), None)
        ]
        
        for start, end, label in arrows:
            self.draw_arrow(ax, start, end, label)
        
        ax.set_title('Tourmate System - Main System Flowchart', 
                    fontsize=16, fontweight='bold', pad=20)
        ax.set_xlim(0, 16)
        ax.set_ylim(-2, 12)
        ax.axis('off')
        
        plt.tight_layout()
        plt.savefig('c:/F-DRIVE/GIT/yuity/main_system_flowchart.png', 
                    dpi=300, bbox_inches='tight')
        plt.show()

    def create_authentication_flowchart(self):
        """Create the authentication module flowchart."""
        fig, ax = plt.subplots(1, 1, figsize=(self.fig_width, self.fig_height))
        
        symbols = [
            ('terminal', 8, 11, 2.5, 0.8, 'START\nAuthentication'),
            ('process', 8, 10, 2.5, 0.8, 'Initialize\nLogin Attempts = 0'),
            ('input_output', 8, 9, 2.5, 0.8, 'Display Login Screen'),
            ('input_output', 8, 8, 2.5, 0.8, 'Get Username\nand Password'),
            ('decision', 8, 7, 2.5, 1, 'Username\nEmpty?'),
            ('input_output', 12, 7, 2, 0.8, 'Display Username\nRequired'),
            ('decision', 8, 5.5, 2.5, 1, 'Password\nEmpty?'),
            ('input_output', 12, 5.5, 2, 0.8, 'Display Password\nRequired'),
            ('process', 8, 4, 2.5, 0.8, 'Validate User\nCredentials'),
            ('decision', 8, 2.8, 2.5, 1, 'Valid\nCredentials?'),
            ('process', 12, 2.8, 2, 0.8, 'Increment\nLogin Attempts'),
            ('process', 4, 2.8, 2, 0.8, 'Set User\nSession'),
            ('decision', 12, 1.5, 2, 1, 'Attempts\n> 3?'),
            ('input_output', 15, 1.5, 1.8, 0.8, 'Lock Account'),
            ('input_output', 12, 0.2, 2, 0.8, 'Display Error\nMessage'),
            ('input_output', 4, 1.5, 2, 0.8, 'Display Welcome\nMessage'),
            ('decision', 12, -1, 2, 1, 'Try Again?'),
            ('terminal', 4, 0.2, 2, 0.8, 'RETURN\nSUCCESS'),
            ('terminal', 15, 0.2, 1.8, 0.8, 'RETURN\nLOCKED'),
            ('terminal', 12, -2.5, 2, 0.8, 'RETURN\nFAILED')
        ]
        
        for symbol_type, x, y, width, height, text in symbols:
            self.create_flowchart_symbol(ax, symbol_type, x, y, width, height, text)
        
        arrows = [
            ((8, 10.6), (8, 10.4), None),
            ((8, 9.6), (8, 9.4), None),
            ((8, 8.6), (8, 8.4), None),
            ((8, 7.6), (8, 7.4), None),
            ((9.25, 7), (12, 7), 'YES'),
            ((8, 6.5), (8, 6), 'NO'),
            ((12, 6.6), (12, 6), None),
            ((12, 6), (8.5, 6), None),
            ((9.25, 5.5), (12, 5.5), 'YES'),
            ((8, 5), (8, 4.4), 'NO'),
            ((12, 5.1), (12, 4.8), None),
            ((12, 4.8), (8.5, 4.8), None),
            ((8, 3.6), (8, 3.3), None),
            ((9.25, 2.8), (12, 2.8), 'NO'),
            ((6.75, 2.8), (4, 2.8), 'YES'),
            ((12, 2.4), (12, 2), None),
            ((12, 2), (15, 2), 'YES'),
            ((12, 1), (12, 0.6), 'NO'),
            ((4, 2.4), (4, 1.9), None),
            ((15, 1.1), (15, 0.6), None),
            ((4, 1.1), (4, 0.6), None),
            ((12, -0.2), (12, -0.5), None),
            ((11, -1), (8.5, 8.5), 'YES'),
            ((12, -1.5), (12, -2.1), 'NO')
        ]
        
        for start, end, label in arrows:
            self.draw_arrow(ax, start, end, label)
        
        ax.set_title('Tourmate System - Authentication Module Flowchart', 
                    fontsize=16, fontweight='bold', pad=20)
        ax.set_xlim(0, 18)
        ax.set_ylim(-3.5, 12)
        ax.axis('off')
        
        plt.tight_layout()
        plt.savefig('c:/F-DRIVE/GIT/yuity/authentication_flowchart.png', 
                    dpi=300, bbox_inches='tight')
        plt.show()

    def create_vehicle_management_flowchart(self):
        """Create the vehicle management module flowchart."""
        fig, ax = plt.subplots(1, 1, figsize=(self.fig_width, self.fig_height))
        
        symbols = [
            ('terminal', 8, 11, 2.5, 0.8, 'START\nVehicle Management'),
            ('input_output', 8, 10, 2.5, 0.8, 'Display Vehicle Menu'),
            ('input_output', 8, 9, 2.5, 0.8, 'Get User Choice'),
            ('decision', 8, 7.5, 3, 1.5, 'Choice\nAnalysis'),
            ('predefined', 2, 6, 2, 0.8, 'Display All\nVehicles'),
            ('predefined', 4.5, 6, 2, 0.8, 'Add Vehicle\nFunction'),
            ('predefined', 7, 6, 2, 0.8, 'Update Vehicle\nFunction'),
            ('predefined', 9.5, 6, 2, 0.8, 'Delete Vehicle\nFunction'),
            ('predefined', 12, 6, 2, 0.8, 'Search Vehicle\nFunction'),
            ('terminal', 14.5, 6, 1.5, 0.8, 'RETURN\nTO MAIN'),
            ('input_output', 8, 4.5, 2.5, 0.8, 'Display Invalid\nChoice Error'),
            ('decision', 8, 3, 2.5, 1, 'Continue?'),
            ('terminal', 8, 1.5, 2, 0.8, 'RETURN\nTO MAIN')
        ]
        
        for symbol_type, x, y, width, height, text in symbols:
            self.create_flowchart_symbol(ax, symbol_type, x, y, width, height, text)
        
        arrows = [
            ((8, 10.6), (8, 10.4), None),
            ((8, 9.6), (8, 9.4), None),
            ((8, 8.6), (8, 8.25), None),
            ((6.5, 7.5), (2, 6.8), '1: View All'),
            ((7, 7.5), (4.5, 6.8), '2: Add New'),
            ((7.5, 7.5), (7, 6.8), '3: Update'),
            ((8.5, 7.5), (9.5, 6.8), '4: Delete'),
            ((9, 7.5), (12, 6.8), '5: Search'),
            ((9.5, 7.5), (14.5, 6.8), '6: Back'),
            ((8, 6.75), (8, 4.9), 'Invalid'),
            ((8, 4.1), (8, 3.5), None),
            ((9.25, 3), (9.5, 8.5), 'YES'),
            ((8, 2.5), (8, 1.9), 'NO'),
            # Return arrows from functions
            ((2, 5.6), (2, 4), None),
            ((2, 4), (6.5, 4), None),
            ((4.5, 5.6), (4.5, 4.2), None),
            ((7, 5.6), (7, 4.4), None),
            ((9.5, 5.6), (9.5, 4.6), None),
            ((12, 5.6), (12, 4.8), None)
        ]
        
        for start, end, label in arrows:
            self.draw_arrow(ax, start, end, label)
        
        ax.set_title('Tourmate System - Vehicle Management Module Flowchart', 
                    fontsize=16, fontweight='bold', pad=20)
        ax.set_xlim(0, 17)
        ax.set_ylim(0, 12)
        ax.axis('off')
        
        plt.tight_layout()
        plt.savefig('c:/F-DRIVE/GIT/yuity/vehicle_management_flowchart.png', 
                    dpi=300, bbox_inches='tight')
        plt.show()

    def create_file_operations_flowchart(self):
        """Create the file operations module flowchart."""
        fig, ax = plt.subplots(1, 1, figsize=(14, 10))
        
        symbols = [
            ('terminal', 7, 9, 2.5, 0.8, 'START\nFile Operations'),
            ('decision', 7, 7.5, 3, 1.5, 'Operation\nType?'),
            ('process', 3, 6, 2, 0.8, 'Check File\nExists'),
            ('process', 7, 6, 2, 0.8, 'Open File'),
            ('process', 11, 6, 2, 0.8, 'Create Backup\nCopy'),
            ('decision', 3, 4.5, 2, 1, 'File\nExists?'),
            ('input_output', 1, 4.5, 1.5, 0.8, 'Display File\nNot Found'),
            ('process', 3, 3, 2, 0.8, 'Create Empty\nRecords'),
            ('decision', 7, 4.5, 2, 1, 'File\nOpen?'),
            ('input_output', 10, 4.5, 2, 0.8, 'Display File\nError'),
            ('process', 7, 3, 2, 0.8, 'Read/Write\nData'),
            ('process', 10, 3, 1.5, 0.8, 'Log Error'),
            ('decision', 7, 1.5, 2.5, 1, 'Operation\nSuccess?'),
            ('input_output', 10.5, 1.5, 2, 0.8, 'Display Operation\nError'),
            ('process', 7, 0, 2, 0.8, 'Close File'),
            ('terminal', 7, -1.5, 2, 0.8, 'RETURN\nSUCCESS')
        ]
        
        for symbol_type, x, y, width, height, text in symbols:
            self.create_flowchart_symbol(ax, symbol_type, x, y, width, height, text)
        
        arrows = [
            ((7, 8.6), (7, 8.25), None),
            ((5.5, 7.5), (3, 6.8), 'LOAD'),
            ((7, 6.75), (7, 6.8), 'SAVE'),
            ((8.5, 7.5), (11, 6.8), 'BACKUP'),
            ((3, 5.6), (3, 5), None),
            ((2, 4.5), (1, 4.5), 'NO'),
            ((3, 4), (3, 3.4), 'YES'),
            ((3, 2.6), (5.5, 2.6), None),
            ((7, 5.6), (7, 5), None),
            ((8, 4.5), (10, 4.5), 'NO'),
            ((7, 4), (7, 3.4), 'YES'),
            ((10, 4.1), (10, 3.4), None),
            ((7, 2.6), (7, 2), None),
            ((8.25, 1.5), (10.5, 1.5), 'NO'),
            ((7, 1), (7, 0.4), 'YES'),
            ((7, -0.4), (7, -1.1), None)
        ]
        
        for start, end, label in arrows:
            self.draw_arrow(ax, start, end, label)
        
        ax.set_title('Tourmate System - File Operations Module Flowchart', 
                    fontsize=16, fontweight='bold', pad=20)
        ax.set_xlim(0, 14)
        ax.set_ylim(-2.5, 10)
        ax.axis('off')
        
        plt.tight_layout()
        plt.savefig('c:/F-DRIVE/GIT/yuity/file_operations_flowchart.png', 
                    dpi=300, bbox_inches='tight')
        plt.show()

def main():
    """Generate all flowcharts for the Tourmate system."""
    print("Generating Tourmate System Flowcharts...")
    
    generator = FlowchartGenerator()
    
    try:
        print("1. Creating Main System Flowchart...")
        generator.create_main_system_flowchart()
        
        print("2. Creating Authentication Module Flowchart...")
        generator.create_authentication_flowchart()
        
        print("3. Creating Vehicle Management Module Flowchart...")
        generator.create_vehicle_management_flowchart()
        
        print("4. Creating File Operations Module Flowchart...")
        generator.create_file_operations_flowchart()
        
        print("\nAll flowcharts generated successfully!")
        print("Files saved:")
        print("- main_system_flowchart.png")
        print("- authentication_flowchart.png")
        print("- vehicle_management_flowchart.png")
        print("- file_operations_flowchart.png")
        
    except Exception as e:
        print(f"Error generating flowcharts: {e}")
        print("Make sure matplotlib is installed: pip install matplotlib")

if __name__ == "__main__":
    main()