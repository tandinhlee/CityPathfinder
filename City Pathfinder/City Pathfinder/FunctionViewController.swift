//
//  SecondViewController.swift
//  City Pathfinder
//
//  Created by Dinh Le on 31/08/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//

import UIKit

class FunctionViewController: UIViewController {
    
    @IBOutlet weak var imageViewSource : UIImageView!
    @IBOutlet weak var imageViewResult : UIImageView!

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func applyButtonTap (obj:AnyObject) {
        print("ButtonTap")
    }


}

