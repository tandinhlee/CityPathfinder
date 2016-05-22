//
//  MyPlaceAnotation.swift
//  ulamdep
//
//  Created by Dinh Le on 12/29/15.
//  Copyright © 2015 UFX. All rights reserved.
//


import Foundation
import MapKit
import AddressBook

class MyPlaceAnotation: NSObject, MKAnnotation {
    let title: String?
    let locationName: String
    var coordinate: CLLocationCoordinate2D
    
    init(title: String, locationName: String, coordinate: CLLocationCoordinate2D) {
        self.title = title
        self.locationName = locationName
        self.coordinate = coordinate
        super.init()
    }
    
    var subtitle: String? {
        return locationName
    }
    // MARK: - MapKit related methods
    // annotation callout opens this mapItem in Maps app
    func mapItem() -> MKMapItem {
        let placemark = MKPlacemark(coordinate: self.coordinate, addressDictionary: nil)
        let mapItem = MKMapItem(placemark: placemark)
        mapItem.name = self.title
        return mapItem
    }
}
