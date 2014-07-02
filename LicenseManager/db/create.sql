#========================================================================== #
# Project Filename:                                                         #
# Project Name:                                                             #
# Author:                                                                   #
# DBMS:                MySQL 4                                              #
# Copyright:                                                                #
# Generated on:        1/8/2006 3:41:52 PM                                 #
#========================================================================== #

#========================================================================== #
#  Tables                                                                   #
#========================================================================== #

CREATE TABLE CustomerTypes (
    ID INTEGER UNSIGNED NOT NULL,
    CustomerType VARCHAR(50) NOT NULL,
    PRIMARY KEY (ID),
    UNIQUE KEY IDX_rule1(id)
);

CREATE TABLE Functionality(
    ID INTEGER UNSIGNED NOT NULL,
    FunctionalityLevel VARCHAR(50) NOT NULL,
    PRIMARY KEY (ID),
    UNIQUE KEY IDX_rule_type1(id)
);

CREATE TABLE KeyVersions (
    ID INTEGER UNSIGNED NOT NULL,
    KeyVersion VARCHAR(50) NOT NULL,
    PRIMARY KEY (ID)
);

CREATE TABLE Products (
    ID INTEGER UNSIGNED NOT NULL,
    ProductName VARCHAR(50) NOT NULL,
    PRIMARY KEY (ID)
);

CREATE TABLE RegData (
    ID INTEGER UNSIGNED NOT NULL AUTO_INCREMENT,
    CustomerName VARCHAR(50) NOT NULL,
    CompanyName VARCHAR(50) NOT NULL,
    CustomerEmail VARCHAR(64) default NULL,
    SerialNumber VARCHAR(50) NOT NULL,
    RegistrationKey VARCHAR (50) NOT NULL,
    DaysValid INTEGER UNSIGNED NOT NULL,
    LicensesPurchased INTEGER UNSIGNED NOT NULL,
    Product INTEGER UNSiGNED NOT NULL,
    CustomerType INTEGER UNSIGNED NOT NULL,
    KeyVersion INTEGER UNSIGNED NOT NULL,
    FunctionalityLevel INTEGER UNSIGNED NOT NULL,
    timeof_creation INTEGER UNSIGNED NOT NULL,
    PRIMARY KEY (ID)
);