//
//  FirstViewController.swift
//  City Pathfinder
//
//  Created by Dinh Le on 31/08/15.
//  Copyright © 2015 Dinh Le. All rights reserved.
//

import UIKit
import MapKit
class MapViewController: UIViewController{
    @IBOutlet weak var mapView: MKMapView!
    var locationManager:CLLocationManager = CLLocationManager()
    var myLocation: MyPlaceAnotation? = nil
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        self.locationManager.delegate = self
        self.locationManager.desiredAccuracy = kCLLocationAccuracyBest
        self.locationManager.requestAlwaysAuthorization()
        self.startUpdatLocation()
    }
    
    
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func locateAction(sender: UIButton) {
        startUpdatLocation()
    }
    
    func centerMapOnLocation(location: CLLocation) {
        let regionRadius: CLLocationDistance = 1000
        let coordinateRegion = MKCoordinateRegionMakeWithDistance(location.coordinate,
                                                                  regionRadius * 2.0, regionRadius * 2.0)
        self.mapView.setRegion(coordinateRegion, animated: true)
        // show artwork on map
        self.locationManager.stopUpdatingLocation()
        if self.myLocation == nil {
            self.myLocation  = MyPlaceAnotation(title: "Dinh Le", locationName: "Dinh's Home"
                , coordinate:location.coordinate)
            mapView.addAnnotation(self.myLocation!)
        } else {
            self.myLocation!.coordinate = location.coordinate
        }
    }
    private func startUpdatLocation() {
        self.locationManager.startUpdatingLocation()
    }
    
    func tapOnInfo(sender: UIButton) {
        debugPrint("tap on button !!!")
    }
    

}
extension MapViewController: MKMapViewDelegate {
    func mapView(mapView: MKMapView,
                 viewForAnnotation annotation: MKAnnotation) -> MKAnnotationView? {
        if let annotation = annotation as? MyPlaceAnotation {
            let identifier = "pin"
            var view: MKAnnotationView
            if let dequeuedView = mapView.dequeueReusableAnnotationViewWithIdentifier(identifier) { // 2
                dequeuedView.annotation = annotation
                view = dequeuedView
            } else {
                // 3
                view = MKAnnotationView(annotation: annotation, reuseIdentifier: identifier)
                view.canShowCallout = true
                view.calloutOffset = CGPoint(x: -5, y: 5)
                let button =  UIButton(type: .InfoDark)
                button.addTarget(self, action: #selector(MapViewController.tapOnInfo(_:)), forControlEvents: .TouchUpInside)
                view.rightCalloutAccessoryView = button as UIView
            }
            view.image = UIImage(named: "ico-your-location")
            return view
        }
        return nil
    }
}

extension MapViewController: CLLocationManagerDelegate {
    func locationManager(manager: CLLocationManager, didUpdateToLocation newLocation: CLLocation, fromLocation oldLocation: CLLocation) {
        self.centerMapOnLocation(newLocation)
    }
}

